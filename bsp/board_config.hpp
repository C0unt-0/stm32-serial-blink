#pragma once

#include "drivers/gpio/gpio.hpp"

#include <stdint.h>

namespace bsp::board_config {

inline constexpr drivers::gpio::Pin user_led_pin{
    drivers::gpio::Port::C,
    13U
};
inline constexpr drivers::gpio::Level user_led_active_level =
    drivers::gpio::Level::Low;

inline constexpr drivers::gpio::Pin user_button_pin{
    drivers::gpio::Port::C,
    15U
};
inline constexpr drivers::gpio::Level user_button_pressed_level =
    drivers::gpio::Level::Low;
inline constexpr drivers::gpio::Pull user_button_pull =
    drivers::gpio::Pull::None;

inline constexpr drivers::gpio::Pin console_uart_tx_pin{
    drivers::gpio::Port::A,
    9U
};
inline constexpr drivers::gpio::Pin console_uart_rx_pin{
    drivers::gpio::Port::A,
    10U
};
inline constexpr uint8_t console_uart_alternate_function = 7U;

}
