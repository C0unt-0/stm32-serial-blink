#include "app/animations/status_led_controller.hpp"

#include "app/time/deadline.hpp"
#include "config/firmware_config.hpp"

namespace app {


void StatusLedController::on()
{
    cancel_blink();
    led_.on();
}

void StatusLedController::off()
{
    cancel_blink();
    led_.off();
}

void StatusLedController::toggle()
{
    cancel_blink();
    led_.toggle();
}

bool StatusLedController::start_blink(uint32_t now_ms)
{
    if (config::led_blink_count == 0U) {
        return false;
    }

    blinking_ = true;
    phase_on_ = true;
    blinks_remaining_ = config::led_blink_count;
    led_.on();
    next_transition_ms_ = now_ms + config::led_on_time_ms;
    return true;
}

void StatusLedController::cancel_blink()
{
    blinking_ = false;
    blinks_remaining_ = 0U;
    phase_on_ = false;
}

bool StatusLedController::update(uint32_t now_ms)
{
    if (!blinking_ ||
        !deadline_reached(now_ms, next_transition_ms_)) {
        return false;
    }

    if (phase_on_) {
        led_.off();
        phase_on_ = false;
        next_transition_ms_ = now_ms + config::led_off_time_ms;
        return false;
    }

    --blinks_remaining_;

    if (blinks_remaining_ == 0U) {
        blinking_ = false;
        return true;
    }

    led_.on();
    phase_on_ = true;
    next_transition_ms_ = now_ms + config::led_on_time_ms;
    return false;
}

bool StatusLedController::is_on() const
{
    return led_.is_on();
}

} // namespace app
