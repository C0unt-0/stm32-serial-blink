#include "platform/stm32f407/registers.hpp"

namespace {

constexpr uintptr_t gpioa_base_address = 0x40020000UL;
constexpr uintptr_t gpio_bank_spacing = 0x00000400UL;
constexpr uintptr_t rcc_base_address = 0x40023800UL;
constexpr uintptr_t usart1_base_address = 0x40011000UL;
constexpr uintptr_t systick_base_address = 0xE000E010UL;
constexpr uint32_t usart1_clock_enable_mask = 1UL << 4U;

constexpr uintptr_t i2c2_base_address = 0x40005800UL;
constexpr uint32_t i2c2_clock_enable_mask = 1UL << 22U;

}

namespace platform::stm32f407 {

GpioRegisters *gpio_registers(uint32_t port_number)
{
    const uintptr_t address =
        gpioa_base_address +
        (gpio_bank_spacing * static_cast<uintptr_t>(port_number));

    return reinterpret_cast<GpioRegisters *>(address);
}

RccRegisters *rcc_registers()
{
    return reinterpret_cast<RccRegisters *>(rcc_base_address);
}

SysTickRegisters *systick_registers()
{
    return reinterpret_cast<SysTickRegisters *>(systick_base_address);
}

UsartPeripheral usart1()
{
    RccRegisters *const rcc = rcc_registers();

    return {
        reinterpret_cast<UsartRegisters *>(usart1_base_address),
        &rcc->APB2ENR,
        usart1_clock_enable_mask
    };
}

I2cPeripheral i2c2()
{
    RccRegisters *const rcc = rcc_registers();

    return {
        reinterpret_cast<I2cRegisters *>(i2c2_base_address),
        &rcc->APB1ENR,
        &rcc->APB1RSTR,
        i2c2_clock_enable_mask,
        i2c2_clock_enable_mask
    };
}

}
