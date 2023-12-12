#pragma once

#include <vector>
#include <cstdint>

namespace Impacto {
namespace Vm {
namespace Dbg {
// Start listening for clients in the background.
void Init();
// Check whether the given address is a breakpoint.
bool IsBreakpoint(uint8_t script_buf, uint32_t addr);
// Notify the client that a breakpoint was hit and wait for a response.
void BreakpointHit(uint8_t script_buf, uint32_t addr);
// Disconnect the client.
void Shutdown();
}  // namespace Dbg
}  // namespace Vm
}  // namespace Impacto