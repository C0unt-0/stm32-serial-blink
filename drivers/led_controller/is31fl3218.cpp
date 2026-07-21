#include "drivers/led_controller/is31fl3218.hpp"

namespace {

constexpr uint8_t device_address = 0x5BU;

constexpr uint8_t shutdown_register = 0x00U;
constexpr uint8_t first_pwm_register = 0x01U;
constexpr uint8_t last_pwm_register = 0x12U;

constexpr uint8_t control_register_1 = 0x13U;
constexpr uint8_t control_register_2 = 0x14U;
constexpr uint8_t control_register_3 = 0x15U;

constexpr uint8_t update_register = 0x16U;
constexpr uint8_t reset_register = 0x17U;

constexpr uint8_t normal_operation = 0x01U;
constexpr uint8_t shutdown = 0x00U;
constexpr uint8_t enable_six_outputs = 0x3FU;

}

namespace drivers {

bool Is31fl3218::write(
    uint8_t register_address,
    uint8_t value)
{
    return i2c_.write_register(
        device_address,
        register_address,
        value) == I2cResult::Ok;
}

bool Is31fl3218::initialize(uint8_t brightness)
{
    if (i2c_.probe(device_address) != I2cResult::Ok) {
        return false;
    }

    if (!write(reset_register, 0U)) {
        return false;
    }

    if (!write(shutdown_register, normal_operation)) {
        return false;
    }

    for (uint8_t reg = first_pwm_register;
         reg <= last_pwm_register;
         ++reg) {
        if (!write(reg, brightness)) {
            return false;
        }
    }

    return
        write(control_register_1, enable_six_outputs) &&
        write(control_register_2, enable_six_outputs) &&
        write(control_register_3, enable_six_outputs) &&
        write(update_register, 0U);
}

bool Is31fl3218::on()
{
    return write(shutdown_register, normal_operation);
}

bool Is31fl3218::off()
{
    return write(shutdown_register, shutdown);
}

bool Is31fl3218::set_channel(
    uint8_t channel,
    uint8_t brightness)
{
    if (channel >= 18U) {
        return false;
    }

    const uint8_t pwm_register =
        static_cast<uint8_t>(
            first_pwm_register + channel);

    return
        write(pwm_register, brightness) &&
        write(update_register, 0U);
}

bool Is31fl3218::clear_all()
{
    for (uint8_t reg = first_pwm_register;
         reg <= last_pwm_register;
         ++reg) {
        if (!write(reg, 0U)) {
            return false;
        }
    }

    return write(update_register, 0U);
}

}