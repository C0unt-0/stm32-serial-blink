#pragma once

#include "drivers/led/led.hpp"

#include <stdint.h>

namespace app {

class StatusLedController final {
public:
    explicit constexpr StatusLedController(drivers::Led &led)
        : led_(led)
    {
    }

    void on();
    void off();
    void toggle();
    bool start_blink(uint32_t now_ms);
    void cancel_blink();
    bool update(uint32_t now_ms);
    bool is_on() const;

private:
    drivers::Led &led_;
    uint32_t next_transition_ms_ = 0U;
    uint32_t blinks_remaining_ = 0U;
    bool blinking_ = false;
    bool phase_on_ = false;
};

} // namespace app
