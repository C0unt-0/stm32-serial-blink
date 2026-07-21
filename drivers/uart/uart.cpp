#include "drivers/uart/uart.hpp"

namespace {

constexpr uint32_t receive_register_not_empty = 1UL << 5U;
constexpr uint32_t transmit_register_empty = 1UL << 7U;
constexpr uint32_t receiver_enable = 1UL << 2U;
constexpr uint32_t transmitter_enable = 1UL << 3U;
constexpr uint32_t usart_enable = 1UL << 13U;

}

namespace drivers {

bool Uart::initialize()
{
    if (hardware_.registers == nullptr ||
        hardware_.clock_enable_register == nullptr ||
        hardware_.clock_enable_mask == 0U ||
        alternate_function_ > 15U ||
        baud_rate_register_ == 0U) {
        return false;
    }

    tx_.configure_alternate(alternate_function_);
    rx_.configure_alternate(alternate_function_);

    *hardware_.clock_enable_register |= hardware_.clock_enable_mask;
    (void)*hardware_.clock_enable_register;

    platform::stm32f407::UsartRegisters *const uart = hardware_.registers;
    uart->CR1 = 0U;
    uart->CR2 = 0U;
    uart->CR3 = 0U;
    uart->BRR = baud_rate_register_;
    uart->CR1 = transmitter_enable | receiver_enable | usart_enable;

    return true;
}

bool Uart::try_read(char &character) const
{
    platform::stm32f407::UsartRegisters *const uart = hardware_.registers;

    if ((uart->SR & receive_register_not_empty) == 0U) {
        return false;
    }

    character = static_cast<char>(uart->DR & 0xFFU);
    return true;
}

void Uart::write(char character) const
{
    platform::stm32f407::UsartRegisters *const uart = hardware_.registers;

    while ((uart->SR & transmit_register_empty) == 0U) {
    }

    uart->DR = static_cast<uint32_t>(
        static_cast<unsigned char>(character));
}

void Uart::write(const char *text) const
{
    if (text == nullptr) {
        return;
    }

    while (*text != '\0') {
        if (*text == '\n') {
            write('\r');
        }

        write(*text);
        ++text;
    }
}

void Uart::write(uint32_t value) const
{
    char buffer[10];
    uint32_t index = sizeof(buffer);

    do {
        const uint32_t digit = value % 10U;

        --index;
        buffer[index] = static_cast<char>('0' + digit);

        value /= 10U;
    } while (value != 0U);

    while (index < sizeof(buffer)) {
        write(buffer[index]);
        ++index;
    }
}

}

