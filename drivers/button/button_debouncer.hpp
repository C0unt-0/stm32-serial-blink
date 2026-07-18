#pragma once

#include <stdint.h>

namespace drivers {

class ButtonDebouncer final {
public:
    explicit constexpr ButtonDebouncer(uint32_t debounce_ms)
        : debounce_ms_(debounce_ms) {}

    void reset(bool pressed, uint32_t now_ms);
    bool update(bool pressed, uint32_t now_ms);
    bool is_pressed() const { return stable_pressed_; }

private:
    uint32_t debounce_ms_;
    uint32_t candidate_since_ms_ = 0U;
    bool stable_pressed_ = false;
    bool candidate_pressed_ = false;
    bool initialized_ = false;
};

}
