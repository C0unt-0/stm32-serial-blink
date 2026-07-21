#pragma once

#include <stdint.h>

namespace platform::stm32f407 {

struct GpioRegisters {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
};

struct RccRegisters {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
};

struct UsartRegisters {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
};

struct I2cRegisters {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
};

struct SysTickRegisters {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
};

struct UsartPeripheral {
    UsartRegisters *registers;
    volatile uint32_t *clock_enable_register;
    uint32_t clock_enable_mask;
};

struct I2cPeripheral {
    I2cRegisters *registers;
    volatile uint32_t *clock_enable_register;
    volatile uint32_t *reset_register;
    uint32_t clock_enable_mask;
    uint32_t reset_mask;
};

GpioRegisters *gpio_registers(uint32_t port_number);
RccRegisters *rcc_registers();
SysTickRegisters *systick_registers();
UsartPeripheral usart1();
I2cPeripheral i2c2();

}
