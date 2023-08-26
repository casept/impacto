#pragma once

// NOTE: Keep in sync w/ sc3ntist
#include <cstdint>
#include <variant>
#include <string>
#include <map>

#include <bitsery/bitsery.h>
#include <bitsery/brief_syntax/variant.h>
#include <bitsery/brief_syntax/map.h>
#include <bitsery/brief_syntax/string.h>
#include <bitsery/brief_syntax.h>

/*
This header defines the debug protocol for talking to a remote VM's types.
It only defines the wire format, it's not a complete implementation.
The messages are sent over TCP, the debugger connects to the target.

Note that each message must also be prepended with the length of serialized data
as a big-endian uint16_t, which can't be done in the `serialize` implementations
because the size of the entire message is only known after serialization
completes.

It's designed such as to keep the target as simple as possible,
loading all the complexity into the debugger instead.
This is because there are multiple targets planned (impacto and the official
engine), but only one debugger.
*/

namespace Impacto::Vm::Dbg::Proto {

using ThreadID = uint8_t;

/** Debugger -> Target **/
namespace Cmd {
enum class Type : uint8_t {
  /// Get a list of threads and their loaded scripts.
  GetThreads,
  /// Get the instruction pointer.
  GetIP,
  /// Get a 32-bit thread-local variable.
  GetLocalVar,
  /// Get a 32-bit global variable.
  GetGlobalVar,
  /// Get a global flag.
  GetGlobalFlag,
  /// Break execution.
  BreakNow,
  /// Continue execution. Target will not send a reply.
  Continue,
  /// Get mapping of script buffers to their scripts.
  GetBuf2ScriptMapping,
  /// Get mapping of script buffers to their threads.
  GetBuf2ThreadMapping,
  /// Set a breakpoint. Target will reply when hit.
  SetBreakpoint,
  /// Set a watchpoint on a local variable. Target will reply when hit.
  SetLocalVarWatchpoint,
  /// Set a watchpoint on a global variable. Target will reply when hit.
  SetGlobalVarWatchpoint,
  /// Set a watchpoint on a global flag. Target will reply when hit.
  SetGlobalFlagWatchpoint,
  /// Unset a breakpoint.
  UnsetBreakpoint,
};

struct GetThreads {
  // Empty is not serializable
  template <typename S>
  void serialize(S& s) {
    s();
  };
};

struct GetIP {
  ThreadID tid;

  template <typename S>
  void serialize(S& s) {
    s(tid);
  };
};

struct GetLocalVar {
  ThreadID tid;
  uint32_t whichVar;

  template <typename S>
  void serialize(S& s) {
    s(tid, whichVar);
  };
};

struct GetGlobalVar {
  uint32_t whichVar;

  template <typename S>
  void serialize(S& s) {
    s(whichVar);
  };
};
;

struct GetGlobalFlag {
  uint32_t whichFlag;

  template <typename S>
  void serialize(S& s) {
    s(whichFlag);
  };
};

struct BreakNow {
  ThreadID tid;

  template <typename S>
  void serialize(S& s) {
    s(tid);
  };
};

struct Continue {
  ThreadID tid;

  template <typename S>
  void serialize(S& s) {
    s(tid);
  };
};

struct SetBreakpoint {
  ThreadID tid;
  uint32_t addr;

  template <typename S>
  void serialize(S& s) {
    s(tid, addr);
  };
};

struct UnsetBreakpoint {
  ThreadID tid;
  uint32_t addr;

  template <typename S>
  void serialize(S& s) {
    s(tid, addr);
  };
};

struct Cmd {
  Type type;
  std::variant<GetThreads, GetIP, GetLocalVar, GetGlobalVar, GetGlobalFlag,
               BreakNow, Continue, SetBreakpoint, UnsetBreakpoint>
      cmd;

  template <typename S>
  void serialize(S& s) {
    s(type, cmd);
  };
};
}  // namespace Cmd

/** Target -> Debugger **/

namespace Reply {

struct GetThreads {
  // TID -> Name of script in it's script buffer
  std::map<std::string, std::vector<ThreadID>> threads;

  template <typename S>
  void serialize(S& s) {
    s(threads);
  };
};

struct GetIP {
  ThreadID tid;
  uint32_t ip;

  template <typename S>
  void serialize(S& s) {
    s(tid, ip);
  };
};

struct GetLocalVar {
  ThreadID tid;
  uint32_t whichVar;
  uint32_t value;

  template <typename S>
  void serialize(S& s) {
    s(tid, whichVar, value);
  };
};

struct GetGlobalVar {
  uint32_t whichVar;
  uint32_t value;

  template <typename S>
  void serialize(S& s) {
    s(whichVar, value);
  };
};
;

struct GetGlobalFlag {
  uint32_t whichFlag;
  bool value;

  template <typename S>
  void serialize(S& s) {
    s(whichFlag, value);
  };
};

struct BreakNow {
  ThreadID tid;

  template <typename S>
  void serialize(S& s) {
    s(tid);
  };
};

struct BreakpointHit {
  ThreadID tid;
  uint32_t addr;

  template <typename S>
  void serialize(S& s) {
    s(tid, addr);
  };
};

enum class Type : uint8_t {
  GetThreads,
  GetIP,
  GetLocalVar,
  GetGlobalVar,
  GetGlobalFlag,
  BreakpointHit
};

struct Reply {
  Type type;
  std::variant<GetThreads, GetIP, GetLocalVar, GetGlobalVar, GetGlobalFlag,
               BreakpointHit>
      reply;

  template <typename S>
  void serialize(S& s) {
    s(type, reply);
  };
};

}  // namespace Reply
}  // namespace Impacto::Vm::Dbg::Proto