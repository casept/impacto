#include "serialize_vm.h"

#include "debugprotocol.pb.h"
#include "../thread.h"
#include "../vm.h"
#include "../../log.h"

#include <cstdint>

namespace Impacto {
namespace Vm {
namespace Dbg {

static size_t getScriptBufRelativeAddr(uint32_t scriptBufId, size_t offset) {
  if (scriptBufId > MaxLoadedScripts) {
    ImpLog(LL_Fatal, LC_VMDbg, "Script buffer ID %u is out of range\n",
           scriptBufId);
  }
  uint8_t* scriptBuf = ScriptBuffers[scriptBufId];
  ssize_t relative = reinterpret_cast<ssize_t>(scriptBuf - offset);
  if (relative < 0) {
    ImpLog(LL_Fatal, LC_VMDbg,
           "Offset points to %zd bytes before the script buffer. "
           "Something's not right.\n",
           relative);
  }
  return static_cast<size_t>(relative);
}

static void serializeThreadState(const Sc3VmThread* t, SC3Debug::VMState& s) {
  // TODO: Expand to all fields
  auto ts = s.add_thread();
  ts->set_tid(t->Id);
  ts->set_flags(t->Flags);
  ts->set_exec_priority(t->ExecPriority);
  ts->set_script_buf(t->ScriptBufferId);
  // TODO: Store name somewhere when loading scripts and remove dummy value
  ts->set_script_name("Dummy");
  ts->set_group_id(t->GroupId);
  ts->set_wait_counter(t->WaitCounter);
  ts->set_script_param(t->ScriptParam);

  // Instruction pointer is stored as it's location in host memory,
  // but debugger expects it to be relative to script buffer base.
  ts->set_ip(getScriptBufRelativeAddr(t->ScriptBufferId,
                                      reinterpret_cast<size_t>(t->Ip)));

  ts->set_loop_counter(t->LoopCounter);
  ts->set_loop_label(t->LoopLabelNum);

  auto cs = ts->mutable_call_stack();
  for (size_t i = 0; i < t->CallStackDepth; i++) {
    /*
    Return address needs to be converted from host pointer to something
    the debugger can interpret as well. This is a bit tricky, as "far calls"
    into other script buffers are possible and need to be handled correctly.
    */
    auto frame = cs->add_return_addr();
    frame->set_script_buf_id(t->ScriptBufferId);
    frame->set_return_offset(getScriptBufRelativeAddr(
        t->ReturnScriptBufferIds[i],
        reinterpret_cast<size_t>(t->ReturnAdresses[i])));
  }

  ts->set_draw_prio(t->DrawPriority);
  ts->set_draw_type(t->DrawType);
  ts->set_alpha(t->Alpha);
  ts->set_temp_1(t->Temp1);
  ts->set_temp_2(t->Temp2);

  for (size_t i; i < MaxThreadVars; i++) {
    ts->add_tlv(t->Variables[i]);
  }

  ts->set_dialogue_page_id(t->DialoguePageId);
}

SC3Debug::VMState serializeVMState() {
  SC3Debug::VMState s{};
  // Walk threads (arranged in a linked list)
  Sc3VmThread* thread = ThreadPool;
  while (thread != nullptr) {
    serializeThreadState(thread, s);
    // TODO: Better iteration (the thread at index 0 might not be the first,
    // e.g. because of death)
    thread = thread->NextContext;
  }
  return s;
}
}  // namespace Dbg
}  // namespace Vm
}  // namespace Impacto
