#include "drivers/button/button_debouncer.hpp"

namespace drivers {

void ButtonDebouncer::reset(bool pressed, uint32_t now_ms)
{
    stable_pressed_ = pressed;
    candidate_pressed_ = pressed;
    candidate_since_ms_ = now_ms;
    initialized_ = true;
}

bool ButtonDebouncer::update(bool pressed, uint32_t now_ms)
{
    if (!initialized_) {
        reset(pressed, now_ms);
        return false;
    }

    if (pressed == stable_pressed_) {
        candidate_pressed_ = pressed;
        candidate_since_ms_ = now_ms;
        return false;
    }

    if (pressed != candidate_pressed_) {
        candidate_pressed_ = pressed;
        candidate_since_ms_ = now_ms;
        return false;
    }

    if (static_cast<uint32_t>(now_ms - candidate_since_ms_) < debounce_ms_) {
        return false;
    }

    const bool was_pressed = stable_pressed_;
    stable_pressed_ = candidate_pressed_;

    return !was_pressed && stable_pressed_;
}

}
