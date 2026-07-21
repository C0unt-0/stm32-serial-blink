#pragma once

#include <stdint.h>

namespace app {

inline bool deadline_reached(
    uint32_t now_ms,
    uint32_t deadline_ms)
{
    return static_cast<int32_t>(now_ms - deadline_ms) >= 0;
}

} // namespace app
