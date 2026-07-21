#pragma once

#include "devices/external_led_board.hpp"
#include "drivers/button/button.hpp"
#include "drivers/gpio/gpio.hpp"
#include "drivers/i2c/i2c.hpp"
#include "drivers/led/led.hpp"
#include "drivers/led_controller/aw9523.hpp"
#include "drivers/uart/uart.hpp"

#include <stdint.h>

namespace bsp {

enum class BoardInitResult : uint8_t {
    Ok,
    UartInitializationFailed,
    I2cInitializationFailed,
    ExternalLedBoardInitializationFailed
};

class Board final {
public:
    Board();

    BoardInitResult initialize(uint32_t now_ms);

    drivers::Led &user_led();
    drivers::Button &user_button();
    drivers::Uart &console_uart();
    devices::ExternalLedBoard &external_led_board();

private:
    static void wait_ms(uint32_t duration_ms);

    drivers::Led user_led_;
    drivers::Button user_button_;
    drivers::Uart console_uart_;
    drivers::I2c led_board_i2c_;
    drivers::gpio::Gpio led_board_reset_;
    drivers::Aw9523 led_controller_;
    devices::ExternalLedBoard external_led_board_;
};

} // namespace bsp
