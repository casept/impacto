#pragma once

#include "debugprotocol.pb.h"

#include <asio.hpp>

#include <cstdint>
#include <optional>
#include <memory>

/*
The actual implementation of the debugger side of the debug protocol.
*/

namespace Impacto::Vm::Dbg::Proto::Impl {
class Connection {
 public:
  Connection() = delete;
  explicit Connection(asio::ip::tcp::socket sock);
  /// Read a command from debugger, if available. Non-blocking.
  std::optional<SC3Debug::Request> RecvCmd();
  /// Send a reply to debugger. Blocks until send completes.
  void SendReply(const SC3Debug::Reply& reply);

 private:
  asio::io_context m_ctx;
  asio::ip::tcp::socket m_sock;
  std::vector<uint8_t> m_recvBuf;
};

class Listener {
 public:
  // Construct listener on the given port.
  explicit Listener(uint16_t port);
  Listener() = delete;
  // Wait for and return a connection.
  std::shared_ptr<Connection> GetConnection();

 private:
  asio::io_context ctx;
  asio::ip::tcp::acceptor acceptor;
};
}  // namespace Impacto::Vm::Dbg::Proto::Impl
