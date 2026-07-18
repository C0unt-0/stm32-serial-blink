#include "drivers/gpio/gpio.hpp"

#include "platform/stm32f407/registers.hpp"

namespace {

bool pin_is_valid(drivers::gpio::Pin pin)
{
    return
        static_cast<uint32_t>(pin.port) <=
            static_cast<uint32_t>(drivers::gpio::Port::I) &&
        pin.number < 16U;
}

constexpr uint32_t bit_mask(uint32_t bit)
{
    return 1UL << bit;
}

constexpr uint32_t two_bit_mask(uint32_t shift)
{
    return 3UL << shift;
}

constexpr uint32_t four_bit_mask(uint32_t shift)
{
    return 15UL << shift;
}

platform::stm32f407::GpioRegisters *registers(drivers::gpio::Port port)
{
    return platform::stm32f407::gpio_registers(
        static_cast<uint32_t>(port));
}

void enable_clock(drivers::gpio::Port port)
{
    platform::stm32f407::RccRegisters *const rcc =
        platform::stm32f407::rcc_registers();

    rcc->AHB1ENR |= bit_mask(static_cast<uint32_t>(port));
    (void)rcc->AHB1ENR;
}

}

namespace drivers::gpio {

void Gpio::configure_output(Level initial_level) const
{
    if (!pin_is_valid(pin_)) {
        return;
    }

    enable_clock(pin_.port);
    write(initial_level);

    platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t pin_index = pin_.number;
    const uint32_t mode_shift = pin_index * 2U;

    gpio->OTYPER &= ~bit_mask(pin_index);
    gpio->OSPEEDR &= ~two_bit_mask(mode_shift);
    gpio->PUPDR &= ~two_bit_mask(mode_shift);
    gpio->MODER =
        (gpio->MODER & ~two_bit_mask(mode_shift)) |
        bit_mask(mode_shift);
}

void Gpio::configure_input(Pull pull) const
{
    if (!pin_is_valid(pin_)) {
        return;
    }

    enable_clock(pin_.port);

    platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t pin_index = pin_.number;
    const uint32_t mode_shift = pin_index * 2U;

    gpio->MODER &= ~two_bit_mask(mode_shift);
    gpio->PUPDR =
        (gpio->PUPDR & ~two_bit_mask(mode_shift)) |
        (static_cast<uint32_t>(pull) << mode_shift);
}

void Gpio::configure_alternate(uint8_t alternate_function) const
{
    if (!pin_is_valid(pin_) || alternate_function > 15U) {
        return;
    }

    enable_clock(pin_.port);

    platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t pin_index = pin_.number;
    const uint32_t mode_shift = pin_index * 2U;
    const uint32_t af_register_index = pin_index / 8U;
    const uint32_t af_shift = (pin_index % 8U) * 4U;

    gpio->OTYPER &= ~bit_mask(pin_index);
    gpio->OSPEEDR =
        (gpio->OSPEEDR & ~two_bit_mask(mode_shift)) |
        (2UL << mode_shift);
    gpio->PUPDR &= ~two_bit_mask(mode_shift);
    gpio->AFR[af_register_index] =
        (gpio->AFR[af_register_index] & ~four_bit_mask(af_shift)) |
        (static_cast<uint32_t>(alternate_function) << af_shift);
    gpio->MODER =
        (gpio->MODER & ~two_bit_mask(mode_shift)) |
        (2UL << mode_shift);
}

void Gpio::write(Level level) const
{
    if (!pin_is_valid(pin_)) {
        return;
    }

    platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t pin_index = pin_.number;

    gpio->BSRR = level == Level::High
        ? bit_mask(pin_index)
        : bit_mask(pin_index + 16U);
}

Level Gpio::read_input() const
{
    if (!pin_is_valid(pin_)) {
        return Level::Low;
    }

    const platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t value = (gpio->IDR >> pin_.number) & 1UL;

    return value != 0U ? Level::High : Level::Low;
}

Level Gpio::read_output() const
{
    if (!pin_is_valid(pin_)) {
        return Level::Low;
    }

    const platform::stm32f407::GpioRegisters *const gpio =
        registers(pin_.port);
    const uint32_t value = (gpio->ODR >> pin_.number) & 1UL;

    return value != 0U ? Level::High : Level::Low;
}

}
