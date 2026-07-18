#pragma once

#include <stdint.h>

namespace platform::stm32f407::system {

void initialize(uint32_t systick_reload_value);
uint32_t milliseconds();

}
