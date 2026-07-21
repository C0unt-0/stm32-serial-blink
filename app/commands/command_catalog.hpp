#pragma once

#include "app/commands/command.hpp"

#include <stdint.h>

namespace app {

struct CommandSpec {
    char key;
    Command command;
    const char *description;
};

inline constexpr CommandSpec command_catalog[] = {
    {'1', Command::StatusLedOn, "status LED on"},
    {'0', Command::StatusLedOff, "status LED off"},
    {'t', Command::ToggleStatusLed, "toggle status LED"},
    {'b', Command::BlinkStatusLed, "blink status LED"},
    {'r', Command::StartRainbow, "start rainbow animation"},
    {'s', Command::StopRainbow, "stop rainbow animation"},
    {'c', Command::ClearExternalLeds, "clear external LEDs"},
    {'?', Command::ShowHelp, "show this command list"}
};

inline constexpr uint32_t command_catalog_size =
    static_cast<uint32_t>(
        sizeof(command_catalog) / sizeof(command_catalog[0]));

} // namespace app
