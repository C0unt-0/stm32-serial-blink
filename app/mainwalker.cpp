// #include "bsp/board.hpp"
// #include "config/firmware_config.hpp"
// #include "drivers/i2c/i2c.hpp"
// #include "drivers/led_controller/aw9523.hpp"
// #include "platform/stm32f407/system.hpp"

// #include <stdint.h>

// namespace {

// constexpr uint8_t led_controller_address = 0x5BU;
// constexpr uint8_t channel_count = 16U;

// /*
//  * Keep the test brightness low during hardware discovery.
//  * Valid AW9523 DIM values are 0–255.
//  */
// constexpr uint8_t test_brightness = 16U;

// constexpr uint32_t reset_time_ms = 10U;
// constexpr uint32_t channel_on_time_ms = 1000U;
// constexpr uint32_t channel_off_time_ms = 250U;

// void wait_ms(uint32_t duration_ms)
// {
//     const uint32_t start =
//         platform::stm32f407::system::milliseconds();

//     while (
//         platform::stm32f407::system::milliseconds() -
//             start <
//         duration_ms) {
//     }
// }

// void write_hex_digit(
//     const drivers::Uart &uart,
//     uint8_t value)
// {
//     if (value < 10U) {
//         uart.write(
//             static_cast<char>('0' + value));
//     } else {
//         uart.write(
//             static_cast<char>(
//                 'A' + value - 10U));
//     }
// }

// void write_hex_byte(
//     const drivers::Uart &uart,
//     uint8_t value)
// {
//     write_hex_digit(
//         uart,
//         static_cast<uint8_t>(
//             (value >> 4U) & 0x0FU));

//     write_hex_digit(
//         uart,
//         static_cast<uint8_t>(
//             value & 0x0FU));
// }

// void write_i2c_result(
//     const drivers::Uart &uart,
//     drivers::I2cResult result)
// {
//     switch (result) {
//     case drivers::I2cResult::Ok:
//         uart.write("OK");
//         break;

//     case drivers::I2cResult::Timeout:
//         uart.write("TIMEOUT");
//         break;

//     case drivers::I2cResult::Nack:
//         uart.write("NACK");
//         break;

//     case drivers::I2cResult::BusError:
//         uart.write("BUS ERROR");
//         break;

//     case drivers::I2cResult::ArbitrationLost:
//         uart.write("ARBITRATION LOST");
//         break;
//     }
// }

// [[noreturn]] void halt()
// {
//     for (;;) {
//     }
// }

// } // namespace

// extern "C" int main()
// {
//     /*
//      * Initialize SysTick.
//      *
//      * This makes system::milliseconds() advance so wait_ms()
//      * can create delays.
//      */
//     platform::stm32f407::system::initialize(
//         config::systick_reload);

//     /*
//      * Initialize the serial console.
//      */
//     drivers::Uart uart =
//         bsp::make_console_uart();

//     uart.initialize();

//     uart.write("\n");
//     uart.write("Starting AW9523 LED channel test...\n");

//     /*
//      * PA6 connects to the external board's RST pin.
//      *
//      * This sequence assumes active-low reset:
//      *
//      *     Low  = reset active
//      *     High = controller running
//      */
//     drivers::gpio::Gpio reset =
//         bsp::make_led_board_shutdown();

//     uart.write("Holding LED board in reset...\n");

//     reset.configure_output(
//         drivers::gpio::Level::Low);

//     wait_ms(reset_time_ms);

//     uart.write("Releasing LED board reset...\n");

//     reset.write(
//         drivers::gpio::Level::High);

//     wait_ms(reset_time_ms);

//     /*
//      * Construct and initialize I2C2.
//      *
//      * PB10 = I2C2_SCL
//      * PB11 = I2C2_SDA
//      */
//     drivers::I2c i2c =
//         bsp::make_led_board_i2c();

//     if (!i2c.initialize()) {
//         uart.write("I2C2 initialization failed.\n");
//         halt();
//     }

//     uart.write("I2C2 initialized.\n");

//     /*
//      * Verify that the external controller acknowledges its
//      * detected 7-bit I2C address.
//      */
//     uart.write("Probing address 0x5B: ");

//     const drivers::I2cResult probe_result =
//         i2c.probe(led_controller_address);

//     write_i2c_result(uart, probe_result);
//     uart.write("\n");

//     if (probe_result != drivers::I2cResult::Ok) {
//         uart.write(
//             "LED controller did not acknowledge address "
//             "0x5B.\n");

//         halt();
//     }

//     uart.write("Address 0x5B acknowledged.\n");

//     /*
//      * Create the AW9523 driver.
//      */
//     drivers::Aw9523 led_board{i2c};

//     uart.write("Initializing AW9523...\n");

//     if (!led_board.initialize()) {
//         uart.write("AW9523 initialization failed.\n");
//         halt();
//     }

//     uart.write("AW9523 initialized.\n");

//     /*
//      * Make sure every current-control channel starts at zero.
//      */
//     if (!led_board.clear_all()) {
//         uart.write("Failed to clear LED channels.\n");
//         halt();
//     }

//     uart.write("All LED channels cleared.\n");
//     uart.write("Starting 16-channel walk.\n");
//     uart.write(
//         "Record which physical LED and color turns on for "
//         "each DIM register.\n");

//     for (;;) {
//         for (uint8_t channel = 0U;
//              channel < channel_count;
//              ++channel) {
//             /*
//              * Ensure only one channel is active.
//              */
//             if (!led_board.clear_all()) {
//                 uart.write(
//                     "Failed to clear LED channels.\n");

//                 halt();
//             }

//             /*
//              * AW9523 channel brightness registers are assumed
//              * to occupy register addresses 0x20–0x2F.
//              */
//             const uint8_t dim_register =
//                 static_cast<uint8_t>(
//                     0x20U + channel);

//             uart.write("Channel 0x");
//             write_hex_byte(uart, channel);

//             uart.write(", DIM register 0x");
//             write_hex_byte(uart, dim_register);

//             uart.write(": ");

//             if (!led_board.set_channel(
//                     channel,
//                     test_brightness)) {
//                 uart.write("FAILED\n");
//                 halt();
//             }

//             uart.write("ON\n");

//             /*
//              * Leave this channel illuminated for one second so
//              * you can identify the physical LED and color.
//              */
//             wait_ms(channel_on_time_ms);

//             if (!led_board.clear_all()) {
//                 uart.write(
//                     "Failed to turn channel off.\n");

//                 halt();
//             }

//             wait_ms(channel_off_time_ms);
//         }

//         uart.write(
//             "Channel walk completed. Repeating...\n");
//     }
// }