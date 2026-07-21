#pragma once

#include "drivers/i2c/i2c.hpp"

#include <stdint.h>

namespace drivers {

class Is31fl3218 final {
public:
    explicit constexpr Is31fl3218(I2c &i2c)
        : i2c_(i2c) {}

    bool initialize(uint8_t brightness);
    bool on();
    bool off();

private:
    bool write(uint8_t register_address, uint8_t value);

    I2c &i2c_;
};

}