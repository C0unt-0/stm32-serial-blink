#include "platform/stm32f407/system.hpp"

#include "platform/stm32f407/registers.hpp"

namespace {

volatile uint32_t tick_count = 0U;
constexpr uint32_t systick_enable = 1UL << 0U;
constexpr uint32_t systick_interrupt_enable = 1UL << 1U;
constexpr uint32_t systick_processor_clock = 1UL << 2U;

}

extern "C" void SysTick_Handler() noexcept
{
    ++tick_count;
}

namespace platform::stm32f407::system {

void initialize(uint32_t systick_reload_value)
{
    SysTickRegisters *const systick = systick_registers();

    tick_count = 0U;
    systick->CTRL = 0U;
    systick->LOAD = systick_reload_value;
    systick->VAL = 0U;
    systick->CTRL =
        systick_processor_clock |
        systick_interrupt_enable |
        systick_enable;
}

uint32_t milliseconds()
{
    return tick_count;
}


}
