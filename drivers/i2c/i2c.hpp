#pragma once

#include "drivers/gpio/gpio.hpp"
#include "platform/stm32f407/registers.hpp"

#include <stdint.h>

namespace drivers {

enum class I2cResult : uint8_t {
    Ok,
    Timeout,
    Nack,
    BusError,
    ArbitrationLost
};

class I2c final {
public:
    constexpr I2c(
        platform::stm32f407::I2cPeripheral hardware,
        gpio::Pin scl_pin,
        gpio::Pin sda_pin,
        uint8_t alternate_function,
        uint32_t peripheral_clock_hz,
        uint32_t bus_frequency_hz,
        uint32_t timeout_iterations)
        : hardware_(hardware),
          scl_(scl_pin),
          sda_(sda_pin),
          alternate_function_(alternate_function),
          peripheral_clock_hz_(peripheral_clock_hz),
          bus_frequency_hz_(bus_frequency_hz),
          timeout_iterations_(timeout_iterations) {}

    bool initialize();

    I2cResult probe(uint8_t address_7bit) const;

    I2cResult write_register(
        uint8_t address_7bit,
        uint8_t register_address,
        uint8_t value) const;

private:
    I2cResult begin_write(uint8_t address_7bit) const;
    I2cResult wait_for_sr1(uint32_t mask) const;
    bool wait_for_bus_free() const;
    void stop() const;
    void clear_errors() const;

    platform::stm32f407::I2cPeripheral hardware_;
    gpio::Gpio scl_;
    gpio::Gpio sda_;
    uint8_t alternate_function_;
    uint32_t peripheral_clock_hz_;
    uint32_t bus_frequency_hz_;
    uint32_t timeout_iterations_;
};



}