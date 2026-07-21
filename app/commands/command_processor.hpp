#pragma once

#include "app/commands/command.hpp"

namespace app {

class CommandProcessor final {
public:
    static Command parse(char character);
};

} // namespace app
