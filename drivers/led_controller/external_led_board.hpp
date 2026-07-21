#pragma once

#include "drivers/led_controller/aw9523.hpp"

#include <stdint.h>

namespace drivers {

enum class ExternalLed : uint8_t {
    D1,
    D2,
    D3,
    D4,
    D5
};

class ExternalLedBoard final {
public:
    explicit constexpr ExternalLedBoard(Aw9523 &controller)
        : controller_(controller)
    {
    }

    bool initialize();

    bool set_rgb(
        ExternalLed led,
        uint8_t red,
        uint8_t green,
        uint8_t blue);

    bool off(ExternalLed led);
    bool clear_all();

private:
    struct Channels {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    static Channels channels_for(ExternalLed led);

    Aw9523 &controller_;
};

} // namespace drivers