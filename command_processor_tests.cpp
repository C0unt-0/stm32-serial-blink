#include "app/command_processor.hpp"

#include <assert.h>

int main()
{
    using app::Command;
    using app::CommandProcessor;

    assert(CommandProcessor::parse('1') == Command::LedOn);
    assert(CommandProcessor::parse('0') == Command::LedOff);
    assert(CommandProcessor::parse('t') == Command::ToggleLed);
    assert(CommandProcessor::parse('T') == Command::ToggleLed);
    assert(CommandProcessor::parse('b') == Command::BlinkLed);
    assert(CommandProcessor::parse('B') == Command::BlinkLed);
    assert(CommandProcessor::parse('?') == Command::ShowHelp);
    assert(CommandProcessor::parse('\n') == Command::None);
    assert(CommandProcessor::parse('x') == Command::Unknown);

    return 0;
}
