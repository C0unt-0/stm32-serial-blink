#pragma once

#include "drivers/button/button_debouncer.hpp"
#include "drivers/gpio/gpio.hpp"

namespace drivers {

class Button final {
public:
    constexpr Button(
        gpio::Pin pin,
        gpio::Level pressed_level,
        gpio::Pull pull,
        uint32_t debounce_ms)
        : gpio_(pin),
          pressed_level_(pressed_level),
          pull_(pull),
          debouncer_(debounce_ms) {}

    void initialize(uint32_t now_ms);
    bool pressed_event(uint32_t now_ms);
    bool is_pressed() const { return debouncer_.is_pressed(); }

private:
    bool raw_pressed() const;

    gpio::Gpio gpio_;
    gpio::Level pressed_level_;
    gpio::Pull pull_;
    ButtonDebouncer debouncer_;
};

}
