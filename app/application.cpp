#include "app/application.hpp"

#include "config/firmware_config.hpp"

namespace {

bool deadline_reached(uint32_t deadline_ms, uint32_t now_ms)
{
    return static_cast<int32_t>(now_ms - deadline_ms) >= 0;
}

}

namespace app {

void Application::initialize(uint32_t now_ms)
{
    led_.initialize();
    button_.initialize(now_ms);
    uart_.initialize();

    uart_.write("Layered C++ UART console ready on USART1.\n");
    print_help();
}

void Application::run_once(uint32_t now_ms)
{
    char character = '\0';


    if (uart_.try_read(character)) {
        process(CommandProcessor::parse(character), now_ms);
    }



    if (button_.pressed_event(now_ms)) {


        uint32_t elapsed = now_ms - last_press_time_ms_;

        if (elapsed <=1000 && elapsed > 0)
        {
            uart_.write("second press detected\n");
            start_blink(now_ms);
        }
        
        ++press_count_;
        uart_.write(press_count_);
        uart_.write("\n");
        cancel_blink();
        led_.toggle();
        last_press_time_ms_ = now_ms;

    }

    update_blink(now_ms);
}

void Application::process(Command command, uint32_t now_ms)
{
    switch (command) {
    case Command::LedOn:
        cancel_blink();
        led_.on();
        report_led_state("");
        break;
    case Command::LedOff:
        cancel_blink();
        led_.off();
        report_led_state("");
        break;
    case Command::ToggleLed:
        cancel_blink();
        led_.toggle();
        report_led_state("");
        break;
    case Command::BlinkLed:
        start_blink(now_ms);
        break;
    case Command::ShowHelp:
        print_help();
        break;
    case Command::Unknown:
        uart_.write("Unknown command. Send ? for help.\n");
        break;
    case Command::None:
        break;
    }
}

void Application::print_help() const
{
    uart_.write(
        "Commands:\n"
        "  1 - LED on\n"
        "  0 - LED off\n"
        "  t - toggle LED\n"
        "  b - blink LED without blocking\n"
        "  ? - show help\n"
        "Button:\n"
        "  PC15 press - toggle LED\n"
    );
}

void Application::report_led_state(const char *prefix) const
{
    uart_.write(prefix);
    uart_.write(led_.is_on() ? "LED on.\n" : "LED off.\n");
}

void Application::start_blink(uint32_t now_ms)
{
    if (config::led_blink_count == 0U) {
        uart_.write("Blink count is zero.\n");
        return;
    }

    blinking_ = true;
    blink_phase_on_ = true;
    blinks_remaining_ = config::led_blink_count;
    led_.on();
    next_blink_transition_ms_ = now_ms + config::led_on_time_ms;
    uart_.write("Blinking LED...\n");
}

void Application::update_blink(uint32_t now_ms)
{
    if (!blinking_ ||
        !deadline_reached(next_blink_transition_ms_, now_ms)) {
        return;
    }

    if (blink_phase_on_) {
        led_.off();
        blink_phase_on_ = false;
        next_blink_transition_ms_ = now_ms + config::led_off_time_ms;
        return;
    }

    --blinks_remaining_;

    if (blinks_remaining_ == 0U) {
        blinking_ = false;
        uart_.write("Blink complete.\n");
        return;
    }

    led_.on();
    blink_phase_on_ = true;
    next_blink_transition_ms_ = now_ms + config::led_on_time_ms;
}

void Application::cancel_blink()
{
    blinking_ = false;
    blinks_remaining_ = 0U;
    blink_phase_on_ = false;
}

}
