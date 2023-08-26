#include "debugprotocol_target.h"

#include <asio.hpp>
#include "debugprotocol.h"
#include <bitsery/adapter/buffer.h>
#include <bitsery/brief_syntax/vector.h>
#include <netinet/tcp.h>

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

using namespace Impacto::Vm::Dbg::Proto::Cmd;

using asio::ip::tcp;
using Buf = std::vector<uint8_t>;
using OutputAdapter = bitsery::OutputBufferAdapter<Buf>;
using InputAdapter = bitsery::InputBufferAdapter<Buf>;

namespace Impacto::Vm::Dbg::Proto::Impl {

static bool byteorder(void) {
  // Alignment required for cast to be always legal
  uint8_t __attribute__((aligned(2))) swaptest[2] = {1, 0};
  if (*(uint16_t*)swaptest == 1) {
    return true;
  }
  return false;
}

static uint16_t byteorder_be_to_u16_native(const uint8_t buf[2]) {
  uint16_t x;
  x = ((uint16_t)buf[0]) << 8;
  x |= ((uint16_t)buf[1]);
  return x;
}

static uint16_t byteorder_le_to_u16_native(const uint8_t buf[2]) {
  uint16_t x;
  x = ((uint16_t)buf[1]) << 8;
  x |= ((uint16_t)buf[0]);
  return x;
}

static uint16_t byteorder_to_u16_native(bool endianness, const uint8_t buf[2]) {
  if (endianness) {
    return byteorder_le_to_u16_native(buf);
  } else {
    return byteorder_be_to_u16_native(buf);
  }
}

static uint16_t byteorder_ne_to_u16_be(uint16_t x) {
  if (!byteorder()) {
    // Machine is already BE
    return x;
  }

  uint16_t y;
  y = (x & 0x00FF) << 8;
  y |= (x & 0xFF00) >> 8;
  return y;
}

Listener::Listener(uint16_t port)
    : ctx({}), acceptor(ctx, tcp::endpoint(tcp::v6(), port)) {
  // Because the boost API makes it impossible to sanely listen on separate v4
  // and v6 acceptors
  asio::ip::v6_only option(false);
  acceptor.set_option(option);
  acceptor.listen();
};

Connection Listener::GetConnection() {
  tcp::socket sock(ctx);
  acceptor.accept(sock);
  return Connection(std::move(sock));
}

Connection::Connection(tcp::socket sock) : ctx({}), sock(ctx), recvBuf({}) {
  // This assumes an asio implementation detail by dealing with raw underlying
  // OS sockets, because apparently it's acceptable for "mature" C++ libraries
  // to lack basic features.
  const int timeoutMs = 1000;
  ::setsockopt(sock.native_handle(), SOL_SOCKET, SO_RCVTIMEO,
               (const char*)&timeoutMs, sizeof timeoutMs);
  ::setsockopt(sock.native_handle(), SOL_SOCKET, SO_SNDTIMEO,
               (const char*)&timeoutMs, sizeof timeoutMs);
  // Reduce latency
  const int one = 1;
  ::setsockopt(sock.native_handle(), SOL_TCP, TCP_NODELAY, &one, sizeof one);
  asio::socket_base::send_buffer_size bufsize(32);
  sock.set_option(bufsize);
}

void Connection::SendReply(const Reply::Reply& reply) {
  Buf buf{};
  const size_t sz = bitsery::quickSerialization<OutputAdapter>(buf, reply);
  // Prepend size
  if (sz > UINT16_MAX) {
    throw std::runtime_error(
        "Connection::SendReply() failed: Message too large");
  }
  const uint16_t sz_u16_be = byteorder_ne_to_u16_be(sz);
  buf.insert(buf.begin(), sz_u16_be & 0x00FF);
  buf.insert(buf.begin(), (sz_u16_be & 0xFF00) >> 8);

  asio::write(sock, asio::buffer(buf));
}

std::optional<Cmd::Cmd> Connection::RecvCmd() {
  // peek is discouraged, so read all bytes into our buffer first
  if (sock.available() > 0) {
    sock.read_some(asio::buffer(recvBuf));
  }
  // Do we at least know the message size?
  if (recvBuf.size() >= 2) {
    // Yes - is the entire message available?
    const uint16_t sz_u16 = byteorder_be_to_u16_native(recvBuf.data());
    const size_t sz = static_cast<size_t>(sz_u16);
    if ((recvBuf.size() - 2) >= sz) {
      Cmd::Cmd cmd{};
      bitsery::quickDeserialization<InputAdapter>({recvBuf.begin(), sz}, cmd);
      recvBuf.erase(recvBuf.begin(), recvBuf.begin() + 2 + sz);
      return {cmd};
    }
  }
  return {};
}
}  // namespace Impacto::Vm::Dbg::Proto::Impl