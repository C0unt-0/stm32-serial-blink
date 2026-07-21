#include "app/animations/rainbow_animation.hpp"

#include "app/time/deadline.hpp"

namespace {

uint8_t scale8(uint8_t value, uint8_t scale)
{
    const uint32_t result =
        static_cast<uint32_t>(value) *
        static_cast<uint32_t>(scale);

    return static_cast<uint8_t>((result + 127U) / 255U);
}

uint8_t gamma_correct(uint8_t value)
{
    const uint32_t squared =
        static_cast<uint32_t>(value) *
        static_cast<uint32_t>(value);

    return static_cast<uint8_t>((squared + 127U) / 255U);
}

uint8_t triangle_wave(uint8_t phase)
{
    if (phase < 128U) {
        return static_cast<uint8_t>(
            static_cast<uint16_t>(phase) * 2U);
    }

    return static_cast<uint8_t>(
        static_cast<uint16_t>(255U - phase) * 2U);
}

devices::Rgb hue_to_rgb(uint8_t hue)
{
    if (hue < 85U) {
        const uint8_t position =
            static_cast<uint8_t>(hue * 3U);

        return {
            static_cast<uint8_t>(255U - position),
            position,
            0U
        };
    }

    if (hue < 170U) {
        const uint8_t position =
            static_cast<uint8_t>((hue - 85U) * 3U);

        return {
            0U,
            static_cast<uint8_t>(255U - position),
            position
        };
    }

    const uint8_t position =
        static_cast<uint8_t>((hue - 170U) * 3U);

    return {
        position,
        0U,
        static_cast<uint8_t>(255U - position)
    };
}

devices::Rgb prepare_color(
    devices::Rgb color,
    uint8_t brightness)
{
    color.red = gamma_correct(scale8(color.red, brightness));
    color.green = gamma_correct(scale8(color.green, brightness));
    color.blue = gamma_correct(scale8(color.blue, brightness));
    return color;
}

} // namespace

namespace app {

void RainbowAnimation::start(uint32_t now_ms)
{
    hue_phase_ = 0U;
    breathing_phase_ = 0U;
    next_frame_ms_ = now_ms;
    running_ = true;
}

void RainbowAnimation::stop()
{
    running_ = false;
}

bool RainbowAnimation::clear()
{
    running_ = false;
    return led_board_.clear_all();
}

bool RainbowAnimation::update(uint32_t now_ms)
{
    if (!running_ ||
        !deadline_reached(now_ms, next_frame_ms_)) {
        return true;
    }

    if (!render_frame()) {
        running_ = false;
        return false;
    }

    hue_phase_ = static_cast<uint8_t>(hue_phase_ + 1U);

    if ((hue_phase_ & 1U) == 0U) {
        breathing_phase_ =
            static_cast<uint8_t>(breathing_phase_ + 1U);
    }

    next_frame_ms_ = now_ms + config_.frame_period_ms;
    return true;
}

bool RainbowAnimation::is_running() const
{
    return running_;
}

bool RainbowAnimation::render_frame()
{
    constexpr uint8_t hue_spacing = 51U;
    constexpr uint8_t breathing_spacing = 20U;

    devices::ExternalLedFrame frame{};

    const uint8_t brightness_range =
        config_.maximum_brightness >= config_.minimum_brightness
            ? static_cast<uint8_t>(
                  config_.maximum_brightness -
                  config_.minimum_brightness)
            : 0U;

    for (uint8_t index = 0U;
         index < devices::ExternalLedFrame::led_count;
         ++index) {
        const uint8_t hue =
            static_cast<uint8_t>(
                hue_phase_ +
                static_cast<uint8_t>(index * hue_spacing));

        const uint8_t local_breathing_phase =
            static_cast<uint8_t>(
                breathing_phase_ +
                static_cast<uint8_t>(index * breathing_spacing));

        const uint8_t brightness =
            static_cast<uint8_t>(
                config_.minimum_brightness +
                scale8(
                    triangle_wave(local_breathing_phase),
                    brightness_range));

        frame.leds[index] =
            prepare_color(hue_to_rgb(hue), brightness);
    }

    return led_board_.show(frame);
}

} // namespace app
