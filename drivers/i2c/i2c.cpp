#include "drivers/i2c/i2c.hpp"

namespace {

constexpr uint32_t cr1_pe = 1UL << 0U;
constexpr uint32_t cr1_start = 1UL << 8U;
constexpr uint32_t cr1_stop = 1UL << 9U;

constexpr uint32_t sr1_sb = 1UL << 0U;
constexpr uint32_t sr1_addr = 1UL << 1U;
constexpr uint32_t sr1_btf = 1UL << 2U;
constexpr uint32_t sr1_txe = 1UL << 7U;
constexpr uint32_t sr1_berr = 1UL << 8U;
constexpr uint32_t sr1_arlo = 1UL << 9U;
constexpr uint32_t sr1_af = 1UL << 10U;
constexpr uint32_t sr1_ovr = 1UL << 11U;
constexpr uint32_t sr1_pecerr = 1UL << 12U;
constexpr uint32_t sr1_timeout = 1UL << 14U;

constexpr uint32_t sr1_error_mask =
    sr1_berr |
    sr1_arlo |
    sr1_af |
    sr1_ovr |
    sr1_pecerr |
    sr1_timeout;

constexpr uint32_t sr2_busy = 1UL << 1U;
constexpr uint32_t cr2_freq_mask = 0x3FUL;
constexpr uint32_t ccr_value_mask = 0x0FFFUL;
constexpr uint32_t trise_mask = 0x3FUL;
constexpr uint32_t oar1_required_bit = 1UL << 14U;

}

namespace drivers {

bool I2c::initialize()
{
    if (hardware_.registers == nullptr ||
        hardware_.clock_enable_register == nullptr ||
        hardware_.reset_register == nullptr ||
        hardware_.clock_enable_mask == 0U ||
        hardware_.reset_mask == 0U ||
        alternate_function_ > 15U ||
        peripheral_clock_hz_ == 0U ||
        bus_frequency_hz_ == 0U ||
        bus_frequency_hz_ > 100'000U ||
        timeout_iterations_ == 0U ||
        (peripheral_clock_hz_ % 1'000'000U) != 0U) {
        return false;
    }

    const uint32_t peripheral_clock_mhz =
        peripheral_clock_hz_ / 1'000'000U;

    if (peripheral_clock_mhz < 2U ||
        peripheral_clock_mhz > cr2_freq_mask) {
        return false;
    }

    const uint32_t denominator = 2U * bus_frequency_hz_;
    const uint32_t ccr =
        (peripheral_clock_hz_ + denominator - 1U) /
        denominator;

    if (ccr < 4U || ccr > ccr_value_mask) {
        return false;
    }

    const uint32_t trise = peripheral_clock_mhz + 1U;

    if (trise > trise_mask) {
        return false;
    }

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
    (void)*hardware_.reset_register;
    *hardware_.reset_register &= ~hardware_.reset_mask;
    (void)*hardware_.reset_register;

    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    i2c->CR1 = 0U;
    i2c->CR2 = peripheral_clock_mhz & cr2_freq_mask;
    i2c->CCR = ccr & ccr_value_mask;
    i2c->TRISE = trise & trise_mask;
    i2c->OAR1 = oar1_required_bit;
    clear_errors();
    i2c->CR1 = cr1_pe;

    return wait_for_bus_free();
}

I2cResult I2c::probe(uint8_t address_7bit) const
{
    const I2cResult result = begin_write(address_7bit);

    if (result != I2cResult::Ok) {
        return result;
    }

    stop();

    return wait_for_bus_free()
        ? I2cResult::Ok
        : I2cResult::Timeout;
}

I2cResult I2c::write_register(
    uint8_t address_7bit,
    uint8_t register_address,
    uint8_t value) const
{
    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    I2cResult result = begin_write(address_7bit);

    if (result != I2cResult::Ok) {
        return result;
    }

    result = wait_for_sr1(sr1_txe);

    if (result != I2cResult::Ok) {
        stop();
        clear_errors();
        return result;
    }

    i2c->DR = register_address;

    result = wait_for_sr1(sr1_txe);

    if (result != I2cResult::Ok) {
        stop();
        clear_errors();
        return result;
    }

    i2c->DR = value;
    result = wait_for_sr1(sr1_btf);

    if (result != I2cResult::Ok) {
        stop();
        clear_errors();
        return result;
    }

    stop();

    return wait_for_bus_free()
        ? I2cResult::Ok
        : I2cResult::Timeout;
}

I2cResult I2c::begin_write(uint8_t address_7bit) const
{
    if (address_7bit > 0x7FU) {
        return I2cResult::BusError;
    }

    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    clear_errors();

    if (!wait_for_bus_free()) {
        return I2cResult::Timeout;
    }

    i2c->CR1 |= cr1_start;

    I2cResult result = wait_for_sr1(sr1_sb);

    if (result != I2cResult::Ok) {
        stop();
        clear_errors();
        return result;
    }

    (void)i2c->SR1;
    i2c->DR = static_cast<uint32_t>(address_7bit) << 1U;

    result = wait_for_sr1(sr1_addr);

    if (result != I2cResult::Ok) {
        stop();
        clear_errors();
        return result;
    }

    (void)i2c->SR1;
    (void)i2c->SR2;

    return I2cResult::Ok;
}

I2cResult I2c::wait_for_sr1(uint32_t mask) const
{
    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    for (uint32_t count = 0U;
         count < timeout_iterations_;
         ++count) {
        const uint32_t status = i2c->SR1;

        if ((status & sr1_timeout) != 0U) {
            return I2cResult::Timeout;
        }

        if ((status & sr1_af) != 0U) {
            return I2cResult::Nack;
        }

        if ((status & sr1_arlo) != 0U) {
            return I2cResult::ArbitrationLost;
        }

        if ((status &
             (sr1_berr | sr1_ovr | sr1_pecerr)) != 0U) {
            return I2cResult::BusError;
        }

        if ((status & mask) == mask) {
            return I2cResult::Ok;
        }
    }

    return I2cResult::Timeout;
}

bool I2c::wait_for_bus_free() const
{
    const platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    for (uint32_t count = 0U;
         count < timeout_iterations_;
         ++count) {
        const bool bus_is_free =
            (i2c->SR2 & sr2_busy) == 0U;

        const bool stop_is_complete =
            (i2c->CR1 & cr1_stop) == 0U;

        if (bus_is_free && stop_is_complete) {
            return true;
        }
    }

    return false;
}

void I2c::stop() const
{
    hardware_.registers->CR1 |= cr1_stop;
}

void I2c::clear_errors() const
{
    platform::stm32f407::I2cRegisters *const i2c =
        hardware_.registers;

    i2c->SR1 &= ~sr1_error_mask;
}

} // namespace drivers