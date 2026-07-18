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

}
