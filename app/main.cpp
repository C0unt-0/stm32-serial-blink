#include "app/animations/rainbow_animation.hpp"
#include "app/animations/status_led_controller.hpp"
#include "app/application.hpp"
#include "app/commands/serial_console.hpp"
#include "bsp/board.hpp"
#include "config/firmware_config.hpp"
#include "platform/stm32f407/system.hpp"

namespace {

[[noreturn]] void halt()
{
    for (;;) {
    }
}

void report_board_failure(
    drivers::Uart &uart,
    bsp::BoardInitResult result)
{
    switch (result) {
    case bsp::BoardInitResult::UartInitializationFailed:
        // The UART may not be usable, but retain a distinct result for
        // debugger-visible diagnosis.
        break;

    case bsp::BoardInitResult::I2cInitializationFailed:
        uart.write("Board initialization failed: I2C2.\n");
        break;

    case bsp::BoardInitResult::ExternalLedBoardInitializationFailed:
        uart.write(
            "Board initialization failed: external LED board.\n");
        break;

    case bsp::BoardInitResult::Ok:
        break;
    }
}

} // namespace

extern "C" int main()
{
    platform::stm32f407::system::initialize(
        config::systick_reload);

    bsp::Board board;

    const uint32_t startup_time_ms =
        platform::stm32f407::system::milliseconds();

    const bsp::BoardInitResult board_result =
        board.initialize(startup_time_ms);

    if (board_result != bsp::BoardInitResult::Ok) {
        report_board_failure(board.console_uart(), board_result);
        halt();
    }

    app::SerialConsole console{board.console_uart()};
    app::StatusLedController status_led{board.user_led()};
    app::RainbowAnimation rainbow{
        board.external_led_board(),
        {
            config::rainbow_minimum_brightness,
            config::rainbow_maximum_brightness,
            config::rainbow_frame_period_ms
        }
    };

    app::Application application{
        console,
        board.user_button(),
        status_led,
        rainbow
    };

    application.initialize(
        platform::stm32f407::system::milliseconds());

    for (;;) {
        application.run_once(
            platform::stm32f407::system::milliseconds());
    }
}
