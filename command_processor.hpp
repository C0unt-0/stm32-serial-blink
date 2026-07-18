#pragma once

namespace app {

enum class Command {
    None,
    LedOn,
    LedOff,
    ToggleLed,
    BlinkLed,
    ShowHelp,
    Unknown
};

class CommandProcessor final {
public:
    static Command parse(char character);
};

}
