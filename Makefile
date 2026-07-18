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

SOURCES := $(shell find app bsp drivers platform startup -name '*.cpp' | sort)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

HOST_FLAGS := -std=c++17 -Wall -Wextra -Werror -O2 -I.

BUTTON_TEST := $(HOST_BUILD_DIR)/button_tests
COMMAND_TEST := $(HOST_BUILD_DIR)/command_processor_tests

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

$(COMMAND_TEST): tests/command_processor_tests.cpp app/command_processor.cpp
	@mkdir -p $(dir $@)
	$(HOST_CXX) $(HOST_FLAGS) $^ -o $@

test: $(BUTTON_TEST) $(COMMAND_TEST)
	$(BUTTON_TEST)
	$(COMMAND_TEST)
	@echo "Host tests passed."

check-layout:
	@test -f app/application.cpp
	@test -f bsp/board_config.hpp
	@test -f drivers/button/button.cpp
	@test -f drivers/led/led.cpp
	@test -f drivers/uart/uart.cpp
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