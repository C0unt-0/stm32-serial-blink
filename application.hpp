#pragma once

#include "app/command_processor.hpp"
#include "drivers/button/button.hpp"
#include "drivers/led/led.hpp"
#include "drivers/uart/uart.hpp"

#include <stdint.h>
#include <cstdio>

namespace app {

class Application final {
public:
    Application(
        drivers::Led &led,
        drivers::Button &button,
        drivers::Uart &uart)
        : led_(led), button_(button), uart_(uart) {}

    void initialize(uint32_t now_ms);
    void run_once(uint32_t now_ms);

private:
    void process(Command command, uint32_t now_ms);
    void print_help() const;
    void report_led_state(const char *prefix) const;
    void start_blink(uint32_t now_ms);
    void update_blink(uint32_t now_ms);
    void cancel_blink();

    drivers::Led &led_;
    drivers::Button &button_;
    drivers::Uart &uart_;

    uint32_t next_blink_transition_ms_ = 0U;
    uint32_t blinks_remaining_ = 0U;
    bool blinking_ = false;
    bool blink_phase_on_ = false;

    uint32_t press_count_ = 0;
};

}
