#pragma once

#include <stdint.h>

namespace config {

inline constexpr uint32_t system_clock_hz = 16'000'000U;
inline constexpr uint32_t systick_frequency_hz = 1'000U;
inline constexpr uint32_t systick_reload =
    (system_clock_hz / systick_frequency_hz) - 1U;

inline constexpr uint32_t uart_baud_rate = 115'200U;
inline constexpr uint32_t usart1_brr =
    (system_clock_hz + (uart_baud_rate / 2U)) / uart_baud_rate;

inline constexpr uint32_t button_debounce_ms = 30U;
inline constexpr uint32_t led_blink_count = 5U;
inline constexpr uint32_t led_on_time_ms = 250U;
inline constexpr uint32_t led_off_time_ms = 250U;

}
