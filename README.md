# STM32F407 layered bare-metal C++ firmware

A C++17 bare-metal STM32F407 project with a non-blocking rainbow animation, UART command console, button input, and a layered hardware/application structure.

## Project structure

```text
app/
    main.cpp                       Composition root only
    application.*                 Event and feature coordination
    animations/                   Timestamp-driven application behaviors
    commands/                     Command catalog, parser, and serial console
    input/                        Hardware-independent input state machines
    time/                         Wraparound-safe time helpers
bsp/                              Board object, wiring, and startup sequencing
config/                           Compile-time firmware policy
devices/                          Board-specific devices and logical frames
drivers/                          Reusable peripheral and IC drivers
platform/stm32f407/               MCU registers, peripherals, and SysTick time
startup/                          Reset handler and interrupt vector table
examples/legacy/                  Old experimental entry points, excluded from builds
tests/                            Hardware-independent host tests
```

## Runtime structure

`app/main.cpp` performs only composition and startup:

1. Initialize SysTick.
2. Construct and initialize `bsp::Board`.
3. Construct application modules from board devices.
4. Call `Application::initialize()`.
5. Repeatedly call `Application::run_once(now_ms)`.

All ongoing behaviors are non-blocking state machines. The UART, button, status LED blink, and rainbow animation remain responsive in the same main loop.

## Application modules

- `SerialConsole` reads characters and prints help from one shared command catalog.
- `StatusLedController` owns onboard LED on/off/toggle/blink behavior.
- `RainbowAnimation` owns hue, breathing, frame timing, and animation state.
- `DoublePressDetector` detects a second press without hardware dependencies.
- `Application` routes commands and button events between these modules.

## External LED layering

```text
RainbowAnimation
    -> devices::ExternalLedBoard
        -> drivers::Aw9523
            -> drivers::I2c
```

`RainbowAnimation` creates a logical five-LED RGB frame. `ExternalLedBoard` owns the physical D1-D5 channel mapping. `Aw9523` owns IC register operations.

## Behavior

The rainbow animation starts automatically after successful board initialization.

UART commands:

- `1`: status LED on
- `0`: status LED off
- `t`: toggle status LED
- `b`: blink status LED without blocking
- `r`: start or restart rainbow animation
- `s`: stop rainbow animation and retain the current frame
- `c`: clear external LEDs
- `?`: display the generated command list

Button behavior:

- Single press: toggle the onboard status LED
- Double press within the configured interval: start the rainbow animation

## Hardware configuration

Board wiring is centralized in `bsp/board_config.hpp`:

- Onboard LED: PC13, active-low
- User button: PC15, active-low
- USART1 TX/RX: PA9/PA10, AF7
- I2C2 SCL/SDA: PB10/PB11, AF4, open-drain
- External LED controller reset: PA6, active-low
- External LED controller address: owned by the AW9523 driver

Timing, brightness, baud rate, debounce, and animation settings are in `config/firmware_config.hpp`.

## Build

With GNU Arm Embedded Toolchain:

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

The production source lists are explicit. Files under `examples/` are not accidentally linked into the firmware.

## Host tests

```sh
make test
```

The tests cover:

- button debounce behavior,
- UART command parsing, including rainbow commands,
- double-press detection.

## Runtime policy

The firmware uses no heap allocation, exceptions, RTTI, hosted runtime, or standard-library containers. Startup initializes `.data` and `.bss`; SysTick provides the millisecond time service.
