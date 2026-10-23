// Hooks for default.xbe. A strong definition replaces the recompiled function at that address, for example:
//
// #include "xdk/hle.h"
//
// X86_EXTERN_FUNC(__imp__sub_00012345);
// X86_FUNC(sub_00012345) {
//     __imp__sub_00012345(ctx, base);  // the original
// }
#include <algorithm>
#include <cstring>

#include "common/options.h"
#include "kernel/memory.h"
#include "xdk/hle.h"

using namespace kernel;

// The library's kick and wait for idle (0x243EE7, called from the texture upload at 0x243C3F): it kicks the
// pushbuffer through 0x244620 and spins until the NV2A reports the channel idle at miniport + 0x100 bit 24,
// which no host code raises because the drawing is the host's. Nothing is left to wait for, and the value it
// hands back is the completion the caller asked for, zero.
X86_FUNC(sub_00243EE7) {
    uint32_t out = R32(ctx.esp + 4);
    if (out) W32(out, 0);
}

// Draw distance. The terrain's detailed tiles, and the scenery lists beside them, are drawn out to one distance a
// stage picks from three constants (15000, 20000 or 25000 units; read at 0x179F30, 0x17A340, 0x17D590 and
// 0x116B20), fading over the last 1000; past it the stage's coarse mesh shows. The tiles walked are a window of
// the 32 by 32 grid around the view, made by 0x1797B0 from the same distance pushed as an immediate (0x178F12 on),
// so both are scaled by the draw_distance setting.
namespace {
constexpr uint32_t kStageDistances[] = {0x2989A4, 0x2989AC, 0x2989B4};

float Reach() { return float(std::clamp(game::Opt().draw_distance, 1u, 4u)); }
}  // namespace

namespace game {
void ScaleDrawDistance() {
    if (Reach() <= 1) return;
    for (uint32_t va : kStageDistances) {
        float d;
        std::memcpy(&d, HostPtr(va), 4);
        d *= Reach();
        std::memcpy(HostPtr(va), &d, 4);
    }
}
}  // namespace game

// (far, stdcall one stack argument after the return address): the grid window of the view out to far
X86_EXTERN_FUNC(__imp__sub_001797B0);
X86_FUNC(sub_001797B0) {
    float far;
    std::memcpy(&far, HostPtr(ctx.esp + 4), 4);
    far *= Reach();
    std::memcpy(HostPtr(ctx.esp + 4), &far, 4);
    __imp__sub_001797B0(ctx, base);
}
