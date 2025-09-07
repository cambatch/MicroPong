MAKE := make

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

BUILD_DIR   := build
DRIVERS_DIR := drivers
DRIVERS_LIB := $(DRIVERS_DIR)/libf446re.a

APP_SRCS := startup/startup.c app/main.c app/ili9341.c
APP_OBJS := $(addprefix $(BUILD_DIR)/,$(APP_SRCS:.c=.o))
APP_DEPS := $(APP_OBJS:.o=.d)

COMMON_WARN := -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion -Wformat-truncation -Wconversion
COMMON_OPT  := -g -O2 -ffunction-sections -fdata-sections -fshort-enums
COMMON_CPU  := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS  ?= $(COMMON_WARN) $(COMMON_OPT) $(COMMON_CPU) -MMD -MP \
           -I$(DRIVERS_DIR)/include $(EXTRA_CFLAGS)

LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs \
           -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/firmware.elf.map

LDLIBS := -L$(DRIVERS_DIR) -lf446re

all: $(BUILD_DIR)/firmware.elf

$(BUILD_DIR)/firmware.elf: $(DRIVERS_LIB) $(APP_OBJS) | $(BUILD_DIR)
	$(CC) $(COMMON_CPU) $(APP_OBJS) -o $@ $(LDFLAGS) \
		-Wl,--start-group $(LDLIBS) -lc -lgcc -Wl,--end-group

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(DRIVERS_LIB):
	$(MAKE) -C $(DRIVERS_DIR)

$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	$(OBJCOPY) -O binary $< $@

firmware.bin: $(BUILD_DIR)/firmware.bin

flash: $(BUILD_DIR)/firmware.bin
	STM32_Programmer_CLI -c port=SWD -d $< 0x08000000 -rst

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(DRIVERS_DIR) clean

.PHONY: all clean flash
-include $(APP_DEPS)
