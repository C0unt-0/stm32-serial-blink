#pragma once

#include <stdint.h>

namespace app {

enum class Command : uint8_t {
    None,
    StatusLedOn,
    StatusLedOff,
    ToggleStatusLed,
    BlinkStatusLed,
    StartRainbow,
    StopRainbow,
    ClearExternalLeds,
    ShowHelp,
    Unknown
};

} // namespace app
