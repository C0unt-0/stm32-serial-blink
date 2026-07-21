#include "bsp/board.hpp"
#include "config/firmware_config.hpp"
#include "drivers/i2c/i2c.hpp"
#include "drivers/led_controller/aw9523.hpp"
#include "drivers/led_controller/external_led_board.hpp"
#include "platform/stm32f407/system.hpp"

#include <stdint.h>

namespace {

constexpr uint8_t led_controller_address = 0x5BU;

/*
 * Keep this conservative during testing.
 *
 * Increase maximum_brightness gradually if the LEDs are too dim.
 * Valid channel values are 0–255.
 */
constexpr uint8_t minimum_brightness = 20U;
constexpr uint8_t maximum_brightness = 150U;

/*
 * One animation frame every 25 ms gives approximately 40 FPS.
 */
constexpr uint32_t frame_time_ms = 5U;
constexpr uint32_t reset_time_ms = 10U;

struct Rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

constexpr drivers::ExternalLed external_leds[] = {
    drivers::ExternalLed::D1,
    drivers::ExternalLed::D2,
    drivers::ExternalLed::D3,
    drivers::ExternalLed::D4,
    drivers::ExternalLed::D5
};

constexpr uint8_t external_led_count =
    static_cast<uint8_t>(
        sizeof(external_leds) /
        sizeof(external_leds[0]));

void wait_ms(uint32_t duration_ms)
{
    const uint32_t start =
        platform::stm32f407::system::milliseconds();

    while (
        platform::stm32f407::system::milliseconds() -
            start <
        duration_ms) {
    }
}

[[noreturn]] void halt()
{
    for (;;) {
    }
}

/*
 * Multiply two 8-bit values and scale the result back to 0–255.
 *
 * Examples:
 *
 * scale8(255, 255) -> 255
 * scale8(255, 128) -> approximately 128
 * scale8(128, 128) -> approximately 64
 */
uint8_t scale8(uint8_t value, uint8_t scale)
{
    const uint32_t result =
        static_cast<uint32_t>(value) *
        static_cast<uint32_t>(scale);

    return static_cast<uint8_t>(
        (result + 127U) / 255U);
}

/*
 * Simple gamma correction.
 *
 * Human vision does not perceive LED brightness linearly.
 * Squaring the value makes low-brightness transitions look smoother.
 */
uint8_t gamma_correct(uint8_t value)
{
    const uint32_t squared =
        static_cast<uint32_t>(value) *
        static_cast<uint32_t>(value);

    return static_cast<uint8_t>(
        (squared + 127U) / 255U);
}

/*
 * Produces a repeating wave:
 *
 * 0 -> 254 -> 0
 *
 * This is used for the gentle breathing effect.
 */
uint8_t triangle_wave(uint8_t phase)
{
    if (phase < 128U) {
        return static_cast<uint8_t>(
            static_cast<uint16_t>(phase) * 2U);
    }

    return static_cast<uint8_t>(
        static_cast<uint16_t>(255U - phase) * 2U);
}

/*
 * Convert a hue value from 0–255 into an RGB rainbow.
 *
 * The three sections are:
 *
 * red   -> green
 * green -> blue
 * blue  -> red
 */
Rgb hue_to_rgb(uint8_t hue)
{
    if (hue < 85U) {
        const uint8_t position =
            static_cast<uint8_t>(hue * 3U);

        return {
            static_cast<uint8_t>(255U - position),
            position,
            0U
        };
    }

    if (hue < 170U) {
        const uint8_t adjusted =
            static_cast<uint8_t>(hue - 85U);

        const uint8_t position =
            static_cast<uint8_t>(adjusted * 3U);

        return {
            0U,
            static_cast<uint8_t>(255U - position),
            position
        };
    }

    const uint8_t adjusted =
        static_cast<uint8_t>(hue - 170U);

    const uint8_t position =
        static_cast<uint8_t>(adjusted * 3U);

    return {
        position,
        0U,
        static_cast<uint8_t>(255U - position)
    };
}

/*
 * Apply global brightness and gamma correction to one RGB color.
 */
Rgb prepare_color(
    Rgb color,
    uint8_t brightness)
{
    color.red = gamma_correct(
        scale8(color.red, brightness));

    color.green = gamma_correct(
        scale8(color.green, brightness));

    color.blue = gamma_correct(
        scale8(color.blue, brightness));

    return color;
}

bool render_rainbow_frame(
    drivers::ExternalLedBoard &led_board,
    uint8_t animation_phase,
    uint8_t breathing_phase)
{
    /*
     * 256 / 5 is approximately 51.
     *
     * This distributes the five LEDs around the full color wheel.
     */
    constexpr uint8_t hue_spacing = 51U;

    /*
     * Slightly offset the breathing wave for each LED.
     * This creates a flowing pulse instead of all LEDs changing
     * brightness at exactly the same time.
     */
    constexpr uint8_t breathing_spacing = 20U;

    for (uint8_t index = 0U;
         index < external_led_count;
         ++index) {
        const uint8_t hue =
            static_cast<uint8_t>(
                animation_phase +
                static_cast<uint8_t>(
                    index * hue_spacing));

        const uint8_t local_breathing_phase =
            static_cast<uint8_t>(
                breathing_phase +
                static_cast<uint8_t>(
                    index * breathing_spacing));

        const uint8_t breathing_value =
            triangle_wave(local_breathing_phase);

        const uint8_t brightness_range =
            static_cast<uint8_t>(
                maximum_brightness -
                minimum_brightness);

        const uint8_t brightness =
            static_cast<uint8_t>(
                minimum_brightness +
                scale8(
                    breathing_value,
                    brightness_range));

        Rgb color = hue_to_rgb(hue);
        color = prepare_color(color, brightness);

        if (!led_board.set_rgb(
                external_leds[index],
                color.red,
                color.green,
                color.blue)) {
            return false;
        }
    }

    return true;
}

} // namespace

extern "C" int main()
{
    /*
     * Configure SysTick so system::milliseconds() works.
     */
    platform::stm32f407::system::initialize(
        config::systick_reload);

    /*
     * Initialize the serial console.
     */
    drivers::Uart uart =
        bsp::make_console_uart();

    uart.initialize();

    uart.write("\n");
    uart.write("Starting external rainbow animation...\n");

    /*
     * Reset the external LED controller.
     *
     * PA6:
     *     Low  = reset active
     *     High = controller running
     */
    drivers::gpio::Gpio reset =
        bsp::make_led_board_shutdown();

    uart.write("Resetting LED controller...\n");

    reset.configure_output(
        drivers::gpio::Level::Low);

    wait_ms(reset_time_ms);

    reset.write(
        drivers::gpio::Level::High);

    wait_ms(reset_time_ms);

    /*
     * Initialize I2C2.
     *
     * PB10 = I2C2_SCL
     * PB11 = I2C2_SDA
     */
    drivers::I2c i2c =
        bsp::make_led_board_i2c();

    if (!i2c.initialize()) {
        uart.write("I2C2 initialization failed.\n");
        halt();
    }

    uart.write("I2C2 initialized.\n");

    /*
     * Check that the detected controller still responds.
     */
    const drivers::I2cResult probe_result =
        i2c.probe(led_controller_address);

    if (probe_result != drivers::I2cResult::Ok) {
        uart.write(
            "LED controller did not acknowledge "
            "address 0x5B.\n");

        halt();
    }

    uart.write("LED controller found at address 0x5B.\n");

    /*
     * Create the low-level AW9523 driver and the
     * board-specific RGB mapping driver.
     */
    drivers::Aw9523 controller{i2c};

    drivers::ExternalLedBoard led_board{
        controller
    };

    if (!led_board.initialize()) {
        uart.write(
            "External LED board initialization failed.\n");

        halt();
    }

    if (!led_board.clear_all()) {
        uart.write(
            "Could not clear external LEDs.\n");

        halt();
    }

    uart.write("Rainbow animation running.\n");

    uint8_t animation_phase = 0U;
    uint8_t breathing_phase = 0U;

    for (;;) {
        if (!render_rainbow_frame(
                led_board,
                animation_phase,
                breathing_phase)) {
            uart.write(
                "I2C failure while rendering animation.\n");

            led_board.clear_all();
            halt();
        }

        /*
         * Advance the color wheel.
         *
         * Increasing by one at 40 FPS gives one complete color
         * rotation in approximately 6.4 seconds.
         */
        animation_phase =
            static_cast<uint8_t>(
                animation_phase + 1U);

        /*
         * Breathing moves more slowly than the hue.
         */
        if ((animation_phase & 1U) == 0U) {
            breathing_phase =
                static_cast<uint8_t>(
                    breathing_phase + 1U);
        }

        wait_ms(frame_time_ms);
    }
}