#include "drivers/button/button.hpp"

namespace drivers {

bool Button::raw_pressed() const
{
    return gpio_.read_input() == pressed_level_;
}

void Button::initialize(uint32_t now_ms)
{
    gpio_.configure_input(pull_);
    debouncer_.reset(raw_pressed(), now_ms);
}

bool Button::pressed_event(uint32_t now_ms)
{
    return debouncer_.update(raw_pressed(), now_ms);
}

}
