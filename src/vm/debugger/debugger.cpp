#include "debugger.h"

#include <atomic>
#include <cstdint>
#include <optional>
#include <thread>
#include <unordered_set>
#include <utility>
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

#include "../../log.h"
#include "debugprotocol_target.h"
#include "debugprotocol.h"

using namespace Impacto::Vm::Dbg::Proto::Impl;
using namespace Impacto::Vm::Dbg::Proto;

static constexpr uint16_t LISTEN_PORT = 1337;

namespace Impacto {
namespace Vm {
namespace Dbg {

struct pair_hash {
  inline std::size_t operator()(const std::pair<uint8_t, uint32_t>& v) const {
    return v.first * 31 + v.second;
  }
};

static std::optional<Connection> conn{};
static std::optional<Listener> listen{};
static std::optional<std::thread> t{};
static std::atomic<bool> shutdown = false;
static std::unordered_set<std::pair<uint8_t, uint32_t>, pair_hash>
    breakpoints{};

void Init() {
  // Block until client connects
  conn.reset();
  listen.reset();
  listen.emplace(LISTEN_PORT);
  ImpLog(LL_Debug, LC_VMDbg, "Waiting for client...\n");
  // Has to be shared_ptr because C++ lambda moves are broken garbage
  auto c = listen.value().GetConnection();
  ImpLog(LL_Debug, LC_VMDbg, "Debugger client connected\n");

  // Async would be a pain, so just do it in a thread
  auto lambda = [c]() mutable {
    ImpLog(LL_Debug, LC_VMDbg, "Starting debugger thread\n");

    // Start handling requests
    while (true) {
      std::this_thread::sleep_for(100ms);
      // Quit if we're shutting down
      if (shutdown) {
        ImpLog(LL_Debug, LC_VMDbg, "Shutting down debugger thread\n");
        return;
      }

      // Otherwise, handle commands, if any
      auto cmd = c->RecvCmd();
      if (cmd.has_value()) {
        ImpLog(LL_Trace, LC_VMDbg, "Got request\n");
        switch (cmd.value().type) {
          case Cmd::Type::SetBreakpoint: {
            auto bp_cmd = std::get<Cmd::SetBreakpoint>(cmd.value().cmd);
            ImpLog(LL_Debug, LC_VMDbg,
                   "Set breakpoint at address %ul in thread %ul\n", bp_cmd.addr,
                   bp_cmd.tid);
            // TODO: Actually set breakpoint
            break;
          }
          case Cmd::Type::GetThreads: {
            ImpLog(LL_Debug, LC_VMDbg, "Get thread list\n");
            // TODO: Get thread list
            // Send empty list for now
            std::map<std::string, std::vector<ThreadID>> threads = {};
            const auto reply = Reply::Reply{
                .type = Reply::Type::GetThreads,
                .reply = {Reply::GetThreads{.threads = threads}},
            };
            c->SendReply(reply);
            ImpLog(LL_Warning, LC_VMDbg, "Sent empty thread list (stub)\n");
            break;
          }

          default: {
            ImpLog(LL_Warning, LC_VMDbg, "Unknown request\n");
            break;
          }
        }
      }
    }
  };
  t.emplace(std::move(lambda));
}

bool IsBreakpoint(uint8_t script_buf, uint32_t addr) {
  return breakpoints.find({script_buf, addr}) != breakpoints.end();
}

static void NotifyBreakpointHit(uint8_t script_buf, uint32_t addr) {
  if (!conn.has_value()) {
    ImpLog(LL_Debug, LC_VMDbg, "No debugger client connected\n");
    return;
  }
  ImpLog(LL_Debug, LC_VMDbg, "Sending breakpoint hit notification\n");
  Reply::Reply r = {
      .type = Reply::Type::BreakpointHit,
      .reply = {Reply::BreakpointHit{.tid = script_buf, .addr = addr}},
  };
  // TODO: Send in thread
  conn.value().SendReply(r);
}

void BreakpointHit(uint8_t script_buf, uint32_t addr) {
  NotifyBreakpointHit(script_buf, addr);
  // TODO: Wait for continue
}

void Shutdown() {
  if (t.has_value()) {
    shutdown = true;
    t.value().join();
  }
  t.reset();
  conn.reset();
  listen.reset();
}

}  // namespace Dbg
}  // namespace Vm
}  // namespace Impacto
