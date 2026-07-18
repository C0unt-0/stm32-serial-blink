# STM32F407 layered bare-metal C++ firmware

A reorganized C++17 bare-metal project for the STM32F407. It keeps board wiring, reusable drivers, MCU-specific register access, startup code, and application behavior in separate layers.

## Project structure

```text
app/                    Product behavior and UART command processing
bsp/                    Board wiring and construction of board devices
config/                 Compile-time firmware settings
drivers/button/         Button device and testable debounce state machine
drivers/gpio/           STM32 GPIO driver
drivers/led/            Reusable active-high/active-low LED driver
drivers/uart/           Polling UART driver
platform/stm32f407/     Register maps, peripheral addresses, and SysTick time
startup/                Reset handler and interrupt vector table
linker/                 STM32F407 linker script
tests/                  Hardware-independent host tests
cmake/                  GNU Arm Embedded CMake toolchain
```

## Hardware configuration

All board pin assignments live in `bsp/board_config.hpp`:

- D4 LED: PC13, active-low
- User button: PC15, active-low, no internal pull resistor
- USART1 TX: PA9, alternate function 7
- USART1 RX: PA10, alternate function 7
- UART: 115200 baud, 8-N-1

The PC15 input assumes the board supplies an external bias resistor. Change `user_button_pull` to `drivers::gpio::Pull::Up` when an internal pull-up is required.

## Behavior

- A debounced button press toggles the LED once.
- Holding the button does not create repeated events.
- UART reception is non-blocking.
- LED blinking is a timestamp-driven state machine, so the button and UART remain responsive while blinking.

UART commands:

- `1`: LED on
- `0`: LED off
- `t`: toggle LED
- `b`: blink LED five times
- `?`: display help

## Build

GNU Arm Embedded Toolchain:

```sh
make
```

Or CMake:

```sh
cmake -S . -B build-cmake \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build-cmake
```

Flash with OpenOCD and ST-Link:

```sh
make flash
```

### Interrupt-handler declarations

The weak interrupt aliases and `Default_Handler` use the same `noexcept` function signature. This is required by newer GNU Arm Embedded compilers when `-Wmissing-attributes` is enabled through `-Wall` and warnings are treated as errors. Do not add `noreturn` only to `Default_Handler`; doing so makes its weak aliases less restrictive and breaks strict GCC builds.

## Host tests

The debounce logic and command parser have no hardware dependencies:

```sh
make test
```

## Runtime policy

The firmware uses no heap allocation, exceptions, RTTI, hosted runtime, or standard-library containers. The reset handler initializes `.data` and `.bss`; the vector table includes all STM32F407 interrupt slots and routes SysTick to the millisecond time service.
