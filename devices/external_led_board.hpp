#pragma once

#include "devices/rgb.hpp"
#include "drivers/led_controller/aw9523.hpp"

#include <stdint.h>

namespace devices {

enum class ExternalLed : uint8_t {
    D1,
    D2,
    D3,
    D4,
    D5
};

struct ExternalLedFrame {
    static constexpr uint8_t led_count = 5U;

    Rgb leds[led_count];
};

class ExternalLedBoard final {
public:
    explicit constexpr ExternalLedBoard(drivers::Aw9523 &controller)
        : controller_(controller)
    {
    }

    bool initialize();

    bool set_rgb(
        ExternalLed led,
        uint8_t red,
        uint8_t green,
        uint8_t blue);

    bool show(const ExternalLedFrame &frame);
    bool off(ExternalLed led);
    bool clear_all();

private:
    struct Channels {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    static Channels channels_for(ExternalLed led);
    static ExternalLed led_for_index(uint8_t index);

    drivers::Aw9523 &controller_;
};

} // namespace devices
