# Architecture

## Dependency direction

```text
startup ───────────────> main
                           │
                           ├──> platform/stm32f407
                           ├──> bsp ───────> platform/stm32f407
                           │      └────────> drivers
                           └──> app ───────> drivers
                                  └────────> config

drivers/gpio ──────────> platform/stm32f407/registers
drivers/uart ──────────> platform/stm32f407/registers
drivers/button ────────> drivers/gpio
drivers/led ───────────> drivers/gpio
```

## Layer rules

### Application

`app/` owns firmware behavior. It receives time as a value and devices as references. It contains no GPIO pin numbers, peripheral base addresses, or RCC bit positions.

### Board support package

`bsp/` is the only layer that knows the board wiring. `board_config.hpp` holds the PC13, PC15, PA9, and PA10 assignments. `board.cpp` connects those assignments to reusable driver objects.

### Drivers

`drivers/` owns device and peripheral behavior. Constructors receive pins, polarity, pull configuration, debounce intervals, and peripheral descriptions rather than relying on hidden global configuration.

The debounce state machine is separated from GPIO access so it can run in host tests.

### Platform

`platform/stm32f407/` owns MCU-specific addresses, register layouts, RCC access, USART1 discovery, and the SysTick millisecond counter.

### Startup

`startup/startup.cpp` initializes `.bss` and `.data`, then enters `main`. `startup/vectors.cpp` contains the complete STM32F407 vector table and weak default handlers. SysTick is the only enabled interrupt in this firmware.

### Configuration

`config/firmware_config.hpp` contains compile-time firmware policy such as clock frequency, UART baud rate, debounce duration, and blink timing.

## Main as composition root

`app/main.cpp` creates the LED, button, and UART through the BSP, injects them into `Application`, and repeatedly passes the current millisecond timestamp to `run_once()`.

No device is accessed through a static singleton. This keeps ownership visible and makes future replacement of a driver or board configuration localized.
