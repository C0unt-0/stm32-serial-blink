#pragma once

#include "drivers/gpio/gpio.hpp"

namespace drivers {

class Led final {
public:
    constexpr Led(gpio::Pin pin, gpio::Level active_level)
        : gpio_(pin), active_level_(active_level) {}

    void initialize();
    void on();
    void off();
    void toggle();
    bool is_on() const;

private:
    gpio::Level inactive_level() const;

    gpio::Gpio gpio_;
    gpio::Level active_level_;
};

}
