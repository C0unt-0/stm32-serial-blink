#include "drivers/led/led.hpp"

namespace drivers {

gpio::Level Led::inactive_level() const
{
    return active_level_ == gpio::Level::High
        ? gpio::Level::Low
        : gpio::Level::High;
}

void Led::initialize()
{
    gpio_.configure_output(inactive_level());
}

void Led::on()
{
    gpio_.write(active_level_);
}

void Led::off()
{
    gpio_.write(inactive_level());
}

void Led::toggle()
{
    is_on() ? off() : on();
}

bool Led::is_on() const
{
    return gpio_.read_output() == active_level_;
}

}
