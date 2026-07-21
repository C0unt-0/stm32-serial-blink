#pragma once

#include "devices/external_led_board.hpp"

#include <stdint.h>

namespace app {

struct RainbowConfig {
    uint8_t minimum_brightness;
    uint8_t maximum_brightness;
    uint32_t frame_period_ms;
};

class RainbowAnimation final {
public:
    constexpr RainbowAnimation(
        devices::ExternalLedBoard &led_board,
        RainbowConfig config)
        : led_board_(led_board),
          config_(config)
    {
    }

    void start(uint32_t now_ms);
    void stop();
    bool clear();
    bool update(uint32_t now_ms);
    bool is_running() const;

private:
    bool render_frame();

    devices::ExternalLedBoard &led_board_;
    RainbowConfig config_;
    uint32_t next_frame_ms_ = 0U;
    uint8_t hue_phase_ = 0U;
    uint8_t breathing_phase_ = 0U;
    bool running_ = false;
};

} // namespace app
