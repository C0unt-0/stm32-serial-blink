#include "drivers/i2c/i2c.hpp"


namespace drivers {

bool I2c::initialize()
{
    scl_.configure_alternate(
        alternate_function_,
        gpio::OutputType::OpenDrain);

    sda_.configure_alternate(
        alternate_function_,
        gpio::OutputType::OpenDrain);

    *hardware_.clock_enable_register |=
        hardware_.clock_enable_mask;

    (void)*hardware_.clock_enable_register;

    *hardware_.reset_register |= hardware_.reset_mask;
    *hardware_.reset_register &= ~hardware_.reset_mask;

    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    const uint32_t peripheral_clock_mhz =
        peripheral_clock_hz_ / 1'000'000U;

    const uint32_t ccr =
        peripheral_clock_hz_ /
        (2U * bus_frequency_hz_);

    i2c->CR1 = 0U;
    i2c->CR2 = peripheral_clock_mhz;
    i2c->CCR = ccr;
    i2c->TRISE = peripheral_clock_mhz + 1U;
    i2c->OAR1 = 1UL << 14U;
    i2c->CR1 = 1UL << 0U;

    return true;
}

I2cResult I2c::write_register(
    uint8_t address_7bit,
    uint8_t register_address,
    uint8_t value) const
{
    platform::stm32f407::I2cRegisters *const i2c = hardware_.registers;

    I2cResult result = begin_write(address_7bit);

    if (result != I2cResult::Ok) {
        return result;
    }

    result = wait_for_sr1(1UL << 7U); // TxE

    if (result != I2cResult::Ok) {
        stop();
        return result;
    }

    i2c->DR = register_address;

    result = wait_for_sr1(1UL << 7U); // TxE

    if (result != I2cResult::Ok) {
        stop();
        return result;
    }

    i2c->DR = value;

    result = wait_for_sr1(1UL << 2U); // BTF
    stop();

    return result;
}



}

