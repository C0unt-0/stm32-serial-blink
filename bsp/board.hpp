#pragma once

#include "drivers/button/button.hpp"
#include "drivers/led/led.hpp"
#include "drivers/uart/uart.hpp"

namespace bsp {

drivers::Led make_user_led();
drivers::Button make_user_button();
drivers::Uart make_console_uart();

}
