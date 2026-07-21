cmake_minimum_required(VERSION 3.20)
project(stm32f407_layered_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FIRMWARE_SOURCES
    app/animations/rainbow_animation.cpp
    app/animations/status_led_controller.cpp
    app/application.cpp
    app/commands/command_processor.cpp
    app/commands/serial_console.cpp
    app/main.cpp
    bsp/board.cpp
    devices/external_led_board.cpp
    drivers/button/button.cpp
    drivers/button/button_debouncer.cpp
    drivers/gpio/gpio.cpp
    drivers/i2c/i2c.cpp
    drivers/led/led.cpp
    drivers/led_controller/aw9523.cpp
    drivers/led_controller/is31fl3218.cpp
    drivers/uart/uart.cpp
    platform/stm32f407/registers.cpp
    platform/stm32f407/system.cpp
    startup/startup.cpp
    startup/vectors.cpp
)

add_executable(${PROJECT_NAME}.elf ${FIRMWARE_SOURCES})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -fno-threadsafe-statics
    -fno-use-cxa-atexit
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -fdata-sections
    -ffunction-sections
    -Wall
    -Wextra
    -Werror
    -O2
    -g3
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -nostdlib
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32f407.ld
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
        $<TARGET_FILE:${PROJECT_NAME}.elf>
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
)
