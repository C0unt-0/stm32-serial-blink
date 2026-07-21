#pragma once

#include <stdint.h>

namespace app {

class DoublePressDetector final {
public:
    explicit constexpr DoublePressDetector(
        uint32_t maximum_interval_ms)
        : maximum_interval_ms_(maximum_interval_ms)
    {
    }

    bool register_press(uint32_t now_ms)
    {
        if (!has_previous_press_) {
            has_previous_press_ = true;
            previous_press_ms_ = now_ms;
            return false;
        }

        const uint32_t elapsed_ms = now_ms - previous_press_ms_;
        previous_press_ms_ = now_ms;

        if (elapsed_ms <= maximum_interval_ms_) {
            has_previous_press_ = false;
            return true;
        }

        return false;
    }

    void reset()
    {
        previous_press_ms_ = 0U;
        has_previous_press_ = false;
    }

private:
    uint32_t maximum_interval_ms_;
    uint32_t previous_press_ms_ = 0U;
    bool has_previous_press_ = false;
};

} // namespace app
