# Architecture

## Dependency direction

```text
startup -> app/main.cpp
              |
              +-> bsp::Board
              |      +-> devices::ExternalLedBoard
              |      |      +-> drivers::Aw9523
              |      |             +-> drivers::I2c
              |      +-> drivers::Led / Button / Uart / Gpio
              |      +-> platform/stm32f407
              |
              +-> app::Application
                     +-> SerialConsole
                     +-> StatusLedController
                     +-> RainbowAnimation
                     +-> DoublePressDetector
```

Dependencies point toward lower-level capabilities. Drivers do not depend on application behavior.

## Layer responsibilities

### `app/main.cpp`: composition root

The production `main()` constructs the object graph, performs board initialization, and repeatedly supplies the current timestamp to the application. It contains no animation math, command switch, LED channel mapping, I2C probing, or frame delay loop.

### `app/application.*`: coordinator

`Application` routes external events:

- parsed serial commands,
- debounced button press events,
- periodic module updates,
- user-facing status messages.

It does not contain UART parsing, blink phase state, rainbow color math, or board wiring.

### `app/animations/`

Active behaviors use the same non-blocking lifecycle shape:

```text
start(now) / direct control operation
update(now)
stop or cancel
```

`RainbowAnimation` calculates logical RGB frames and schedules them with timestamps. `StatusLedController` schedules onboard LED blink transitions. Neither waits in a runtime delay loop.

### `app/commands/`

`command_catalog.hpp` is the single source of truth for command keys, command values, and help descriptions. The parser and help output both consume this catalog.

`SerialConsole` adapts the concrete UART into application-level command polling and text output.

### `app/input/`

`DoublePressDetector` contains only timestamp state. It has no GPIO dependency and is host-testable.

### `bsp/`

`bsp::Board` owns the concrete board devices and their dependency order. It is the only aggregate that knows which devices exist together on this board.

`Board::initialize()` performs startup-only hardware sequencing:

1. initialize UART, onboard LED, and button,
2. assert and release the external controller reset line,
3. initialize I2C,
4. initialize the external LED device.

`board_config.hpp` owns all pin assignments and electrical polarity.

### `devices/`

A device combines one or more generic drivers into a board/product-level concept.

`ExternalLedBoard` owns:

- the D1-D5 logical LED identifiers,
- the mapping from each RGB component to AW9523 channels,
- logical frame presentation through `show(frame)`.

This prevents the animation from depending on physical channel numbers.

### `drivers/`

Drivers own peripheral or IC behavior:

- GPIO, UART, I2C,
- onboard LED and button wrappers,
- AW9523 and IS31FL3218 register protocols.

The AW9523 driver owns its I2C address and probing behavior. The application does not probe device addresses directly.

### `platform/stm32f407/`

The platform layer owns STM32F407 register layouts, peripheral descriptions, RCC register addresses, and the SysTick millisecond counter.

### `config/`

`firmware_config.hpp` contains product policy rather than wiring:

- clock and baud settings,
- debounce and double-press intervals,
- blink timing,
- rainbow brightness and frame period,
- reset and I2C timeouts.

## Initialization versus runtime

Blocking waits are restricted to the startup reset sequence in `bsp::Board::initialize()`. Runtime features are timestamp-driven.

Hardware configuration is validated before I2C pin and peripheral side effects. Initialization functions report failure where hardware discovery or configuration can fail.

## Build structure

The Makefile and CMake project list production sources explicitly. Experimental mains are stored under `examples/legacy/` and are not part of firmware builds.
