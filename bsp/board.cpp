#include "bsp/board.hpp"

#include "bsp/board_config.hpp"
#include "config/firmware_config.hpp"
#include "platform/stm32f407/registers.hpp"
#include "platform/stm32f407/system.hpp"

namespace bsp {

Board::Board()
    : user_led_(
          board_config::user_led_pin,
          board_config::user_led_active_level),
      user_button_(
          board_config::user_button_pin,
          board_config::user_button_pressed_level,
          board_config::user_button_pull,
          config::button_debounce_ms),
      console_uart_(
          platform::stm32f407::usart1(),
          board_config::console_uart_tx_pin,
          board_config::console_uart_rx_pin,
          board_config::console_uart_alternate_function,
          config::usart1_brr),
      led_board_i2c_(
          platform::stm32f407::i2c2(),
          board_config::led_board_i2c_scl_pin,
          board_config::led_board_i2c_sda_pin,
          board_config::led_board_i2c_alternate_function,
          config::apb1_clock_hz,
          board_config::led_board_i2c_frequency_hz,
          config::i2c_timeout_iterations),
      led_board_reset_(board_config::led_board_reset_pin),
      led_controller_(led_board_i2c_),
      external_led_board_(led_controller_)
{
}

BoardInitResult Board::initialize(uint32_t now_ms)
{
    if (!console_uart_.initialize()) {
        return BoardInitResult::UartInitializationFailed;
    }

    user_led_.initialize();
    user_button_.initialize(now_ms);

    led_board_reset_.configure_output(
        board_config::led_board_reset_active_level);

    wait_ms(config::led_controller_reset_time_ms);

    led_board_reset_.write(
        board_config::led_board_reset_inactive_level);

    wait_ms(config::led_controller_reset_time_ms);

    if (!led_board_i2c_.initialize()) {
        return BoardInitResult::I2cInitializationFailed;
    }

    if (!external_led_board_.initialize()) {
        return BoardInitResult::ExternalLedBoardInitializationFailed;
    }

    return BoardInitResult::Ok;
}

drivers::Led &Board::user_led()
{
    return user_led_;
}

drivers::Button &Board::user_button()
{
    return user_button_;
}

drivers::Uart &Board::console_uart()
{
    return console_uart_;
}

devices::ExternalLedBoard &Board::external_led_board()
{
    return external_led_board_;
}

void Board::wait_ms(uint32_t duration_ms)
{
    const uint32_t start_ms =
        platform::stm32f407::system::milliseconds();

    while (
        platform::stm32f407::system::milliseconds() - start_ms <
        duration_ms) {
    }
}

} // namespace bsp
