#pragma once

#include "drivers/button/button.hpp"
#include "drivers/gpio/gpio.hpp"
#include "drivers/i2c/i2c.hpp"
#include "drivers/led/led.hpp"
#include "drivers/uart/uart.hpp"

namespace bsp {

drivers::Led make_user_led();
drivers::Button make_user_button();
drivers::Uart make_console_uart();

drivers::I2c make_led_board_i2c();
drivers::gpio::Gpio make_led_board_shutdown();

} // namespace bsp