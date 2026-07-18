#pragma once

#include "drivers/gpio/gpio.hpp"
#include "platform/stm32f407/registers.hpp"

#include <stdint.h>

namespace drivers {

class Uart final {
public:
    constexpr Uart(
        platform::stm32f407::UsartPeripheral hardware,
        gpio::Pin tx_pin,
        gpio::Pin rx_pin,
        uint8_t alternate_function,
        uint32_t baud_rate_register)
        : hardware_(hardware),
          tx_(tx_pin),
          rx_(rx_pin),
          alternate_function_(alternate_function),
          baud_rate_register_(baud_rate_register) {}

    void initialize();
    bool try_read(char &character) const;
    void write(char character) const;
    void write(const char *text) const;
    void write(uint32_t value) const;

private:
    platform::stm32f407::UsartPeripheral hardware_;
    gpio::Gpio tx_;
    gpio::Gpio rx_;
    uint8_t alternate_function_;
    uint32_t baud_rate_register_;
};

}
