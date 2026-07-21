#include "drivers/led_controller/external_led_board.hpp"

namespace drivers {

ExternalLedBoard::Channels
ExternalLedBoard::channels_for(ExternalLed led)
{
    switch (led) {
    case ExternalLed::D1:
        return {
            4U, // red:   register 0x24
            5U, // green: register 0x25
            6U  // blue:  register 0x26
        };

    case ExternalLed::D2:
        return {
            7U, // red:   register 0x27
            8U, // green: register 0x28
            9U  // blue:  register 0x29
        };

    case ExternalLed::D3:
        return {
            10U, // red:   register 0x2A
            11U, // green: register 0x2B
            0U   // blue:  register 0x20
        };

    case ExternalLed::D4:
        return {
            1U, // red:   register 0x21
            2U, // green: register 0x22
            3U  // blue:  register 0x23
        };

    case ExternalLed::D5:
        return {
            12U, // red:   register 0x2C
            13U, // green: register 0x2D
            14U  // blue:  register 0x2E
        };
    }

    return {0U, 0U, 0U};
}

bool ExternalLedBoard::initialize()
{
    return controller_.initialize();
}

bool ExternalLedBoard::set_rgb(
    ExternalLed led,
    uint8_t red,
    uint8_t green,
    uint8_t blue)
{
    const Channels channels =
        channels_for(led);

    return
        controller_.set_channel(channels.red, red) &&
        controller_.set_channel(channels.green, green) &&
        controller_.set_channel(channels.blue, blue);
}

bool ExternalLedBoard::off(ExternalLed led)
{
    return set_rgb(led, 0U, 0U, 0U);
}

bool ExternalLedBoard::clear_all()
{
    return controller_.clear_all();
}

} // namespace drivers