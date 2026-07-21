#include "app/commands/command_processor.hpp"

#include "app/commands/command_catalog.hpp"

#include <stdint.h>

namespace {

char normalize(char character)
{
    if (character >= 'A' && character <= 'Z') {
        return static_cast<char>(character - 'A' + 'a');
    }

    return character;
}

} // namespace

namespace app {

Command CommandProcessor::parse(char character)
{
    if (character == '\r' || character == '\n') {
        return Command::None;
    }

    const char normalized = normalize(character);

    for (uint32_t index = 0U;
         index < command_catalog_size;
         ++index) {
        if (command_catalog[index].key == normalized) {
            return command_catalog[index].command;
        }
    }

    return Command::Unknown;
}

} // namespace app
