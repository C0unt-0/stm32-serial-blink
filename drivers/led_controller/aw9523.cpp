#include "drivers/led_controller/aw9523.hpp"

namespace {

constexpr uint8_t device_address = 0x5BU;

/*
 * AW9523 control registers.
 */
constexpr uint8_t global_control_register = 0x11U;
constexpr uint8_t port0_led_mode_register = 0x12U;
constexpr uint8_t port1_led_mode_register = 0x13U;

/*
 * The sixteen LED-current registers occupy 0x20–0x2F.
 */
constexpr uint8_t first_dim_register = 0x20U;
constexpr uint8_t last_dim_register = 0x2FU;
constexpr uint8_t channel_count = 16U;

/*
 * LED mode register:
 *
 * bit = 0 -> LED current-control mode
 * bit = 1 -> ordinary GPIO mode
 */
constexpr uint8_t all_channels_led_mode = 0x00U;

/*
 * Select a conservative global current range.
 *
 * The lowest two bits control the global maximum current.
 */
constexpr uint8_t low_global_current_range = 0x03U;

} // namespace

namespace drivers {

bool Aw9523::write(
    uint8_t register_address,
    uint8_t value)
{
    return i2c_.write_register(
        device_address,
        register_address,
        value) == I2cResult::Ok;
}

bool Aw9523::initialize()
{
    if (i2c_.probe(device_address) != I2cResult::Ok) {
        return false;
    }

    /*
     * Select a low global-current range for initial testing.
     */
    if (!write(
            global_control_register,
            low_global_current_range)) {
        return false;
    }

    /*
     * Configure all 16 outputs as LED-current outputs.
     */
    if (!write(
            port0_led_mode_register,
            all_channels_led_mode)) {
        return false;
    }

    if (!write(
            port1_led_mode_register,
            all_channels_led_mode)) {
        return false;
    }

    return clear_all();
}

bool Aw9523::clear_all()
{
    for (uint8_t reg = first_dim_register;
         reg <= last_dim_register;
         ++reg) {
        if (!write(reg, 0U)) {
            return false;
        }
    }

    return true;
}

bool Aw9523::set_channel(
    uint8_t channel,
    uint8_t brightness)
{
    if (channel >= channel_count) {
        return false;
    }

    const uint8_t dim_register =
        static_cast<uint8_t>(
            first_dim_register + channel);

    return write(dim_register, brightness);
}

} // namespace drivers