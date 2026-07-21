#include "app/commands/command_processor.hpp"

#include <assert.h>

int main()
{
    using app::Command;
    using app::CommandProcessor;

    assert(CommandProcessor::parse('1') == Command::StatusLedOn);
    assert(CommandProcessor::parse('0') == Command::StatusLedOff);
    assert(CommandProcessor::parse('t') == Command::ToggleStatusLed);
    assert(CommandProcessor::parse('T') == Command::ToggleStatusLed);
    assert(CommandProcessor::parse('b') == Command::BlinkStatusLed);
    assert(CommandProcessor::parse('B') == Command::BlinkStatusLed);
    assert(CommandProcessor::parse('r') == Command::StartRainbow);
    assert(CommandProcessor::parse('R') == Command::StartRainbow);
    assert(CommandProcessor::parse('s') == Command::StopRainbow);
    assert(CommandProcessor::parse('S') == Command::StopRainbow);
    assert(CommandProcessor::parse('c') == Command::ClearExternalLeds);
    assert(CommandProcessor::parse('C') == Command::ClearExternalLeds);
    assert(CommandProcessor::parse('?') == Command::ShowHelp);
    assert(CommandProcessor::parse('\r') == Command::None);
    assert(CommandProcessor::parse('\n') == Command::None);
    assert(CommandProcessor::parse('x') == Command::Unknown);

    return 0;
}
