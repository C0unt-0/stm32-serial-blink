PROJECT := firmware
BUILD_DIR := build
HOST_BUILD_DIR := build-host

CXX := arm-none-eabi-g++
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
HOST_CXX ?= g++

CPU_FLAGS := -mcpu=cortex-m4 -mthumb

CXXFLAGS := $(CPU_FLAGS) \
            -std=c++17 \
            -ffreestanding \
            -fno-exceptions \
            -fno-rtti \
            -fno-threadsafe-statics \
            -fno-use-cxa-atexit \
            -fno-unwind-tables \
            -fno-asynchronous-unwind-tables \
            -fdata-sections \
            -ffunction-sections \
            -Wall -Wextra -Werror \
            -O2 -g3 \
            -I.

LDFLAGS := $(CPU_FLAGS) \
           -nostdlib \
           -Wl,--gc-sections \
           -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map \
           -Tlinker/stm32f407.ld

SOURCES := \
    app/animations/rainbow_animation.cpp \
    app/animations/status_led_controller.cpp \
    app/application.cpp \
    app/commands/command_processor.cpp \
    app/commands/serial_console.cpp \
    app/main.cpp \
    bsp/board.cpp \
    devices/external_led_board.cpp \
    drivers/button/button.cpp \
    drivers/button/button_debouncer.cpp \
    drivers/gpio/gpio.cpp \
    drivers/i2c/i2c.cpp \
    drivers/led/led.cpp \
    drivers/led_controller/aw9523.cpp \
    drivers/led_controller/is31fl3218.cpp \
    drivers/uart/uart.cpp \
    platform/stm32f407/registers.cpp \
    platform/stm32f407/system.cpp \
    startup/startup.cpp \
    startup/vectors.cpp

OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

HOST_FLAGS := -std=c++17 -Wall -Wextra -Werror -O2 -I.

BUTTON_TEST := $(HOST_BUILD_DIR)/button_tests
COMMAND_TEST := $(HOST_BUILD_DIR)/command_processor_tests
DOUBLE_PRESS_TEST := $(HOST_BUILD_DIR)/double_press_detector_tests

.PHONY: all build-flash clean flash test check-layout

all: check-layout $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin
	$(SIZE) $(BUILD_DIR)/$(PROJECT).elf

build-flash: all flash
	@echo "Firmware built and flashed successfully."

$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS) linker/stm32f407.ld
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUTTON_TEST): tests/button_tests.cpp drivers/button/button_debouncer.cpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_FLAGS) $^ -o $@

$(COMMAND_TEST): tests/command_processor_tests.cpp app/commands/command_processor.cpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_FLAGS) $^ -o $@

$(DOUBLE_PRESS_TEST): tests/double_press_detector_tests.cpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_FLAGS) $^ -o $@

test: $(BUTTON_TEST) $(COMMAND_TEST) $(DOUBLE_PRESS_TEST)
	$(BUTTON_TEST)
	$(COMMAND_TEST)
	$(DOUBLE_PRESS_TEST)
	@echo "Host tests passed."

check-layout:
	@test -f app/application.cpp
	@test -f app/animations/rainbow_animation.cpp
	@test -f app/commands/command_catalog.hpp
	@test -f app/input/double_press_detector.hpp
	@test -f bsp/board.cpp
	@test -f devices/external_led_board.cpp
	@test -f drivers/i2c/i2c.cpp
	@test -f platform/stm32f407/registers.cpp
	@test -f startup/vectors.cpp
	@echo "Layered project layout check passed."

flash: $(BUILD_DIR)/$(PROJECT).elf
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $< verify reset exit"

clean:
	rm -rf $(BUILD_DIR) $(HOST_BUILD_DIR)

-include $(DEPS)
