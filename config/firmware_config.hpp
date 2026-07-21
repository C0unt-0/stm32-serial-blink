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
inline constexpr uint32_t double_press_interval_ms = 2'000U;

inline constexpr uint32_t led_blink_count = 5U;
inline constexpr uint32_t led_on_time_ms = 250U;
inline constexpr uint32_t led_off_time_ms = 250U;

inline constexpr uint8_t rainbow_minimum_brightness = 20U;
inline constexpr uint8_t rainbow_maximum_brightness = 150U;
inline constexpr uint32_t rainbow_frame_period_ms = 25U;

inline constexpr uint32_t led_controller_reset_time_ms = 10U;

inline constexpr uint32_t apb1_clock_hz = system_clock_hz;
inline constexpr uint32_t i2c_timeout_iterations = 100'000U;

} // namespace config
