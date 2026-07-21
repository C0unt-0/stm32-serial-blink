#pragma once

#include "app/animations/rainbow_animation.hpp"
#include "app/animations/status_led_controller.hpp"
#include "app/commands/command.hpp"
#include "app/commands/serial_console.hpp"
#include "app/input/double_press_detector.hpp"
#include "config/firmware_config.hpp"
#include "drivers/button/button.hpp"

#include <stdint.h>

namespace app {

class Application final {
public:
    Application(
        SerialConsole &console,
        drivers::Button &button,
        StatusLedController &status_led,
        RainbowAnimation &rainbow)
        : console_(console),
          button_(button),
          status_led_(status_led),
          rainbow_(rainbow),
          double_press_(config::double_press_interval_ms)
    {
    }

    void initialize(uint32_t now_ms);
    void run_once(uint32_t now_ms);

private:
    void process_command(Command command, uint32_t now_ms);
    void process_button(uint32_t now_ms);
    void report_status_led() const;

    SerialConsole &console_;
    drivers::Button &button_;
    StatusLedController &status_led_;
    RainbowAnimation &rainbow_;
    DoublePressDetector double_press_;
};

} // namespace app
