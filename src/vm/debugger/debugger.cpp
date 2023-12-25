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
#include "debugprotocol.pb.h"

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
        switch (cmd.value().type()) {
          case SC3Debug::REQUEST_TYPE_GET_STATE: {
            ImpLog(LL_Debug, LC_VMDbg, "Debugger requested VM state\n");
            // TODO: Process and return state
            break;
          }

          default: {
            ImpLog(LL_Warning, LC_VMDbg, "Unknown or unimplemented request\n");
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
  ImpLog(LL_Debug, LC_VMDbg,
         "Sending breakpoint hit notification not yet implemented\n");
  // TODO: Implement
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
