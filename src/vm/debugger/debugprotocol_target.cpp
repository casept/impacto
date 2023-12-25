#include "debugprotocol_target.h"

#include <asio.hpp>
#include "debugprotocol.pb.h"
#include <netinet/tcp.h>

#include "../../log.h"

// Part of the ugly timeout hack
#if defined(WIN32) || defined(_WIN32) || \
    defined(__WIN32) && !defined(__CYGWIN__)
#include <winsock.h>
#else
extern "C" {
#include <sys/socket.h>
}
#endif

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <memory>
#include <sstream>
#include <iomanip>

using asio::ip::tcp;
using Buf = std::vector<uint8_t>;

namespace Impacto::Vm::Dbg::Proto::Impl {

Listener::Listener(uint16_t port)
    : ctx({}), acceptor(ctx, tcp::endpoint(tcp::v6(), port)) {
  // Because the boost API makes it impossible to sanely listen on separate v4
  // and v6 acceptors
  // FIXME: Apparently this is an unsupported option?
  // asio::ip::v6_only option(false);
  // acceptor.set_option(option);

  acceptor.listen();
};

std::shared_ptr<Connection> Listener::GetConnection() {
  tcp::socket sock(ctx);
  acceptor.accept(sock);
  return std::make_shared<Connection>(std::move(sock));
}

Connection::Connection(tcp::socket sock)
    : m_ctx({}), m_sock(std::move(sock)), m_recvBuf({}) {
  // This assumes an asio implementation detail by dealing with raw underlying
  // OS sockets, because apparently it's acceptable for "mature" C++ libraries
  // to lack basic features.
  const int timeoutMs = 1000;
  ::setsockopt(m_sock.native_handle(), SOL_SOCKET, SO_RCVTIMEO,
               (const char*)&timeoutMs, sizeof timeoutMs);
  ::setsockopt(m_sock.native_handle(), SOL_SOCKET, SO_SNDTIMEO,
               (const char*)&timeoutMs, sizeof timeoutMs);
  // Reduce latency
  const int one = 1;
  ::setsockopt(m_sock.native_handle(), SOL_TCP, TCP_NODELAY, &one, sizeof one);
  asio::socket_base::send_buffer_size bufsize(32);
  m_sock.set_option(bufsize);
}

void Connection::SendReply(const SC3Debug::Reply& reply) {
  if (!reply.IsInitialized()) {
    ImpLog(LL_Error, LC_VMDbg,
           "Attempt to send uninitialized message, dropping!\n");
    return;
  }
  ImpLog(LL_Trace, LC_VMDbg, "Sending debug protocol reply: %s\n",
         reply.DebugString().c_str());

  Buf buf{};
  size_t size = reply.ByteSizeLong();
  buf.resize(size + 4);

  google::protobuf::io::ArrayOutputStream* arr =
      new google::protobuf::io::ArrayOutputStream(buf.data(), 4 + size);
  google::protobuf::io::CodedOutputStream* coded =
      new google::protobuf::io::CodedOutputStream(arr);
  coded->WriteLittleEndian32(size);
  reply.SerializeToCodedStream(coded);

  ImpLog(LL_Debug, LC_VMDbg, "Sending %zu bytes\n", buf.size());
  size_t transferred = asio::write(m_sock, asio::buffer(buf));
  if (transferred != buf.size()) {
    ImpLog(LL_Warning, LC_VMDbg,
           "Failed to transfer all data to debugger! Wanted to send %zu bytes, "
           "sent %zu\n",
           buf.size(), transferred);
  }

  delete coded;
  delete arr;
}

std::optional<SC3Debug::Request> Connection::RecvCmd() {
  // peek is discouraged, so read all bytes into our buffer first
  const size_t avail = m_sock.available();
  if (avail > 0) {
    ImpLog(LL_Trace, LC_VMDbg, "Received %zu bytes from client\n", avail);
    m_recvBuf.resize(m_recvBuf.size() + avail);
    m_sock.read_some(asio::buffer(m_recvBuf));
  }

  google::protobuf::io::ArrayInputStream* arr =
      new google::protobuf::io::ArrayInputStream(m_recvBuf.data(),
                                                 m_recvBuf.size());
  google::protobuf::io::CodedInputStream* coded =
      new google::protobuf::io::CodedInputStream(arr);

  // Check whether we have enough data to determine size
  std::optional<size_t> size{};
  if (m_recvBuf.size() >= 4) {
    uint32_t size32;
    coded->ReadLittleEndian32(&size32);
    size.emplace(static_cast<size_t>(size32));
    ImpLog(LL_Trace, LC_VMDbg,
           "Awaiting message body of %zu bytes from client\n", size.value());
    coded->SetTotalBytesLimit(size.value() + 4);
  }

  // Check whether message is complete
  if (size.has_value() && coded->BytesUntilTotalBytesLimit() >= size) {
    // Message is complete - read and clear buffer
    SC3Debug::Request r = SC3Debug::Request();
    r.ParseFromCodedStream(coded);
    // Have to create a new vector and swap, as bytes are in front
    Buf newBuf{};
    const auto left = coded->BytesUntilTotalBytesLimit();
    newBuf.resize(left);
    std::copy(m_recvBuf.begin() + m_recvBuf.size() - left, m_recvBuf.end(),
              newBuf.begin());
    m_recvBuf.swap(newBuf);
    delete coded;
    delete arr;
    return {r};
  }

  delete coded;
  delete arr;
  return {};
}
}  // namespace Impacto::Vm::Dbg::Proto::Impl
