#include "app/command_processor.hpp"

namespace app {

Command CommandProcessor::parse(char character)
{
    switch (character) {
    case '1':
        return Command::LedOn;
    case '0':
        return Command::LedOff;
    case 't':
    case 'T':
        return Command::ToggleLed;
    case 'b':
    case 'B':
        return Command::BlinkLed;
    case '?':
        return Command::ShowHelp;
    case '\r':
    case '\n':
        return Command::None;
    default:
        return Command::Unknown;
    }
}

}
