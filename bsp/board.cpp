#include "bsp/board.hpp"

#include "bsp/board_config.hpp"
#include "config/firmware_config.hpp"
#include "platform/stm32f407/registers.hpp"

namespace bsp {

drivers::Led make_user_led()
{
    return drivers::Led{
        board_config::user_led_pin,
        board_config::user_led_active_level
    };
}

drivers::Button make_user_button()
{
    return drivers::Button{
        board_config::user_button_pin,
        board_config::user_button_pressed_level,
        board_config::user_button_pull,
        config::button_debounce_ms
    };
}

drivers::Uart make_console_uart()
{
    return drivers::Uart{
        platform::stm32f407::usart1(),
        board_config::console_uart_tx_pin,
        board_config::console_uart_rx_pin,
        board_config::console_uart_alternate_function,
        config::usart1_brr
    };
}

drivers::I2c make_led_board_i2c()
{
    return drivers::I2c{
        platform::stm32f407::i2c2(),
        board_config::led_board_i2c_scl_pin,
        board_config::led_board_i2c_sda_pin,
        board_config::led_board_i2c_alternate_function,
        config::apb1_clock_hz,
        board_config::led_board_i2c_frequency_hz,
        config::i2c_timeout_iterations
    };
}

drivers::gpio::Gpio make_led_board_shutdown()
{
    return drivers::gpio::Gpio{
        board_config::led_board_reset_pin
    };
}

}
