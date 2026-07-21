#pragma once

#include "app/commands/command.hpp"
#include "drivers/uart/uart.hpp"

namespace app {

class SerialConsole final {
public:
    explicit constexpr SerialConsole(drivers::Uart &uart)
        : uart_(uart)
    {
    }

    void show_startup() const;
    Command poll() const;
    void print_help() const;
    void write(const char *text) const;

private:
    drivers::Uart &uart_;
};

} // namespace app
