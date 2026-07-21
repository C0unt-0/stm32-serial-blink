#include "bsp/board.hpp"
#include "config/firmware_config.hpp"
#include "drivers/led_controller/is31fl3218.hpp"
#include "platform/stm32f407/system.hpp"

namespace {

void wait_ms(uint32_t duration_ms)
{
    const uint32_t start =
        platform::stm32f407::system::milliseconds();

    while (
        platform::stm32f407::system::milliseconds() -
        start < duration_ms) {
    }
}

}

extern "C" int main()
{
    platform::stm32f407::system::initialize(
        config::systick_reload);

    drivers::Uart uart = bsp::make_console_uart();
    uart.initialize();

    uart.write("Starting external LED test...\n");

    drivers::gpio::Gpio shutdown =
        bsp::make_led_board_shutdown();

    /*
     * The board labels this RST. If the controller is an
     * IS31FL3218, it is actually the active-low SDB
     * hardware-shutdown input.
     */
    shutdown.configure_output(
        drivers::gpio::Level::Low);

    wait_ms(10U);

    shutdown.write(drivers::gpio::Level::High);

    wait_ms(10U);

    drivers::I2c i2c = bsp::make_led_board_i2c();

    if (!i2c.initialize()) {
        uart.write("I2C2 initialization failed.\n");

        for (;;) {
        }
    }

    uart.write("I2C2 initialized.\n");

    drivers::Is31fl3218 led_board{i2c};

    if (!led_board.initialize(16U)) {
        uart.write(
            "No LED controller response at address 0x54.\n");

        for (;;) {
        }
    }

    uart.write("LED controller initialized.\n");

    for (;;) {
        led_board.on();
        uart.write("External LEDs on.\n");
        wait_ms(500U);

        led_board.off();
        uart.write("External LEDs off.\n");
        wait_ms(500U);
    }
}