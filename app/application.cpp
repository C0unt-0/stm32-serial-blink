#include "app/application.hpp"

namespace app {

void Application::initialize(uint32_t now_ms)
{
    console_.show_startup();
    rainbow_.start(now_ms);
    console_.write("Rainbow animation started.\n");
}

void Application::run_once(uint32_t now_ms)
{
    process_command(console_.poll(), now_ms);
    process_button(now_ms);

    if (status_led_.update(now_ms)) {
        console_.write("Blink complete.\n");
    }

    if (!rainbow_.update(now_ms)) {
        console_.write(
            "Rainbow animation stopped: external LED write failed.\n");
    }
}

void Application::process_command(
    Command command,
    uint32_t now_ms)
{
    switch (command) {
    case Command::StatusLedOn:
        status_led_.on();
        report_status_led();
        break;

    case Command::StatusLedOff:
        status_led_.off();
        report_status_led();
        break;

    case Command::ToggleStatusLed:
        status_led_.toggle();
        report_status_led();
        break;

    case Command::BlinkStatusLed:
        if (status_led_.start_blink(now_ms)) {
            console_.write("Blinking status LED...\n");
        } else {
            console_.write("Blink count is zero.\n");
        }
        break;

    case Command::StartRainbow:
        rainbow_.start(now_ms);
        console_.write("Rainbow animation started.\n");
        break;

    case Command::StopRainbow:
        rainbow_.stop();
        console_.write("Rainbow animation stopped; frame retained.\n");
        break;

    case Command::ClearExternalLeds:
        if (rainbow_.clear()) {
            console_.write("External LEDs cleared.\n");
        } else {
            console_.write("Failed to clear external LEDs.\n");
        }
        break;

    case Command::ShowHelp:
        console_.print_help();
        break;

    case Command::Unknown:
        console_.write("Unknown command. Send ? for help.\n");
        break;

    case Command::None:
        break;
    }
}

void Application::process_button(uint32_t now_ms)
{
    if (!button_.pressed_event(now_ms)) {
        return;
    }

    if (double_press_.register_press(now_ms)) {
        rainbow_.start(now_ms);
        console_.write("Double press: rainbow animation started.\n");
        return;
    }

    status_led_.toggle();
    report_status_led();
}

void Application::report_status_led() const
{
    console_.write(
        status_led_.is_on()
            ? "Status LED on.\n"
            : "Status LED off.\n");
}

} // namespace app
