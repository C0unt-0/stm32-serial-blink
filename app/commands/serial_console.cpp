#include "app/commands/serial_console.hpp"

#include "app/commands/command_catalog.hpp"
#include "app/commands/command_processor.hpp"

#include <stdint.h>

namespace app {

void SerialConsole::show_startup() const
{
    uart_.write("Layered C++ firmware ready on USART1.\n");
    print_help();
}

Command SerialConsole::poll() const
{
    char character = '\0';

    if (!uart_.try_read(character)) {
        return Command::None;
    }

    return CommandProcessor::parse(character);
}

void SerialConsole::print_help() const
{
    uart_.write("Commands:\n");

    for (uint32_t index = 0U;
         index < command_catalog_size;
         ++index) {
        uart_.write("  ");
        uart_.write(command_catalog[index].key);
        uart_.write(" - ");
        uart_.write(command_catalog[index].description);
        uart_.write("\n");
    }

    uart_.write(
        "Button:\n"
        "  PC15 press - toggle status LED\n"
        "  PC15 double press - start rainbow\n");
}

void SerialConsole::write(const char *text) const
{
    uart_.write(text);
}

} // namespace app
