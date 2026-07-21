#pragma once

#include "drivers/i2c/i2c.hpp"

#include <stdint.h>

namespace drivers {

class Aw9523 final {
public:
    explicit constexpr Aw9523(I2c &i2c)
        : i2c_(i2c)
    {
    }

    bool initialize();

    bool clear_all();

    bool set_channel(
        uint8_t channel,
        uint8_t brightness);

private:
    bool write(
        uint8_t register_address,
        uint8_t value);

    I2c &i2c_;
};

} // namespace drivers