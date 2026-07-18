# Validation report

Validation performed on the packaged source tree:

- Host C++17 compilation with `-Wall -Wextra -Werror`
- GCC warning-sensitive compilation of `startup/vectors.cpp` passed
- Host debounce tests passed
- Host UART command-parser tests passed
- Freestanding ARM Cortex-M4 compilation with Clang 17
- Static link with LLD and the included STM32F407 linker script
- No unresolved symbols in the linked ELF
- Vector table size: 392 bytes, equal to 98 32-bit entries
- Initial stack pointer: `0x20020000`
- Reset vector is a Thumb address in flash
- SysTick vector is a Thumb address in flash
- Linked binary payload after the handler-signature fix: 2,684 bytes; BSS: 4 bytes

The GNU Arm Embedded commands in the Makefile and CMake toolchain were not run in this environment because `arm-none-eabi-g++` was unavailable. The same sources were target-compiled and linked for Cortex-M4 using Clang/LLD. Physical board behavior was not tested.

The interrupt aliases were additionally checked for the GNU `-Wmissing-attributes` diagnostic that occurs when a weak alias has a less restrictive exception or return contract than `Default_Handler`.
