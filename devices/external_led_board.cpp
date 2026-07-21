#include "devices/external_led_board.hpp"

namespace devices {

ExternalLedBoard::Channels
ExternalLedBoard::channels_for(ExternalLed led)
{
    switch (led) {
    case ExternalLed::D1:
        return {4U, 5U, 6U};
    case ExternalLed::D2:
        return {7U, 8U, 9U};
    case ExternalLed::D3:
        return {10U, 11U, 0U};
    case ExternalLed::D4:
        return {1U, 2U, 3U};
    case ExternalLed::D5:
        return {12U, 13U, 14U};
    }

    return {0U, 0U, 0U};
}

ExternalLed ExternalLedBoard::led_for_index(uint8_t index)
{
    switch (index) {
    case 0U:
        return ExternalLed::D1;
    case 1U:
        return ExternalLed::D2;
    case 2U:
        return ExternalLed::D3;
    case 3U:
        return ExternalLed::D4;
    case 4U:
        return ExternalLed::D5;
    default:
        return ExternalLed::D1;
    }
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
    const Channels channels = channels_for(led);

    return
        controller_.set_channel(channels.red, red) &&
        controller_.set_channel(channels.green, green) &&
        controller_.set_channel(channels.blue, blue);
}

bool ExternalLedBoard::show(const ExternalLedFrame &frame)
{
    for (uint8_t index = 0U;
         index < ExternalLedFrame::led_count;
         ++index) {
        const Rgb color = frame.leds[index];

        if (!set_rgb(
                led_for_index(index),
                color.red,
                color.green,
                color.blue)) {
            return false;
        }
    }

    return true;
}

bool ExternalLedBoard::off(ExternalLed led)
{
    return set_rgb(led, 0U, 0U, 0U);
}

bool ExternalLedBoard::clear_all()
{
    return controller_.clear_all();
}

} // namespace devices
