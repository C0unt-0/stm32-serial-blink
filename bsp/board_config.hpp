#pragma once

#include "drivers/gpio/gpio.hpp"

#include <stdint.h>

namespace bsp::board_config {

/*
 * Onboard user LED
 */
inline constexpr drivers::gpio::Pin user_led_pin{
    drivers::gpio::Port::C,
    13U
};

inline constexpr drivers::gpio::Level user_led_active_level =
    drivers::gpio::Level::Low;

/*
 * Onboard user button
 */
inline constexpr drivers::gpio::Pin user_button_pin{
    drivers::gpio::Port::C,
    15U
};

inline constexpr drivers::gpio::Level user_button_pressed_level =
    drivers::gpio::Level::Low;

inline constexpr drivers::gpio::Pull user_button_pull =
    drivers::gpio::Pull::None;

/*
 * Console UART
 *
 * PA9  = USART1_TX
 * PA10 = USART1_RX
 * AF7  = USART1 alternate function
 */
inline constexpr drivers::gpio::Pin console_uart_tx_pin{
    drivers::gpio::Port::A,
    9U
};

inline constexpr drivers::gpio::Pin console_uart_rx_pin{
    drivers::gpio::Port::A,
    10U
};

inline constexpr uint8_t console_uart_alternate_function = 7U;

/*
 * External LED board I2C connection
 *
 * PB10 = I2C2_SCL
 * PB11 = I2C2_SDA
 * AF4  = I2C alternate function
 */
inline constexpr drivers::gpio::Pin led_board_i2c_scl_pin{
    drivers::gpio::Port::B,
    10U
};

inline constexpr drivers::gpio::Pin led_board_i2c_sda_pin{
    drivers::gpio::Port::B,
    11U
};

inline constexpr uint8_t led_board_i2c_alternate_function = 4U;

inline constexpr drivers::gpio::OutputType
    led_board_i2c_output_type =
        drivers::gpio::OutputType::OpenDrain;

inline constexpr uint32_t led_board_i2c_frequency_hz =
    100'000U;

/*
 * External LED board reset
 *
 * PA6 is connected to the board's RST pin.
 * Reset is assumed to be active-low:
 *
 * Low  = board held in reset
 * High = board operating normally
 */
inline constexpr drivers::gpio::Pin led_board_reset_pin{
    drivers::gpio::Port::A,
    6U
};

inline constexpr drivers::gpio::Level
    led_board_reset_active_level =
        drivers::gpio::Level::Low;

inline constexpr drivers::gpio::Level
    led_board_reset_inactive_level =
        drivers::gpio::Level::High;

} // namespace bsp::board_config