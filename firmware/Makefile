# Toolchain
CROSS     ?= arm-none-eabi-
CC        := $(CROSS)gcc
AS        := $(CROSS)gcc
LD        := $(CROSS)gcc
OBJCOPY   := $(CROSS)objcopy
SIZE      := $(CROSS)size
MAKE	  := make
FLASH     := STM32_Programmer_CLI

APP_DIR        := app
STARTUP_DIR    := startup
BUILD_DIR      := build
APP_BUILD_DIR  := $(BUILD_DIR)/app
STARTUP_BUILD  := $(BUILD_DIR)/startup

DRIVERS_DIR    := drivers
DRIVERS_LIB    := $(DRIVERS_DIR)/libf446re.a

TARGET     := firmware
ELF        := $(BUILD_DIR)/$(TARGET).elf
BIN        := $(BUILD_DIR)/$(TARGET).bin
MAP        := $(BUILD_DIR)/$(TARGET).elf.map
LINKER     := link.ld

# MCU/ABI flags
MCUFLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
COMMON   := -ffunction-sections -fdata-sections -fshort-enums
WARN     := -W -Wall -Wextra -Werror
OPT      := -O2
STD      := -std=c11

CFLAGS   := $(MCUFLAGS) $(COMMON) $(WARN) $(OPT) $(STD)
ASFLAGS  := $(MCUFLAGS) $(COMMON)
LDFLAGS  := $(MCUFLAGS) -T $(LINKER) -Wl,-Map=$(MAP) -Wl,--gc-sections -nostartfiles

# Include paths
INCLUDES := -I. -Iinclude -I$(APP_DIR)/display -I$(DRIVERS_DIR)/include

# Sources
APP_CS   := $(wildcard $(APP_DIR)/*.c) \
			$(wildcard $(APP_DIR)/display/*.c)
STARTUP_S:= $(STARTUP_DIR)/startup.s

APP_OBJS     := $(patsubst $(APP_DIR)/%.c,$(APP_BUILD_DIR)/%.o,$(APP_CS))
STARTUP_OBJ  := $(STARTUP_BUILD)/startup.o

LD_OBJS := $(STARTUP_OBJ) $(APP_OBJS)

LD_LIBS := $(DRIVERS_LIB)

# ---------------------------------------------------------------------------

.PHONY: all clean drivers size

all: $(BUILD_DIR) drivers $(ELF) $(BIN) size

$(BUILD_DIR):
	@mkdir -p $(APP_BUILD_DIR) $(STARTUP_BUILD)

drivers:
	$(MAKE) -C $(DRIVERS_DIR)

# Compile C sources
$(APP_BUILD_DIR)/%.o: $(APP_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(STARTUP_BUILD)/startup.o: $(STARTUP_S)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(ELF): $(LD_OBJS) $(LINKER) $(DRIVERS_LIB)
	$(LD) $(LDFLAGS) $(LD_OBJS) $(LD_LIBS) -o $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD_DIR)/firmware.bin
	$(FLASH) -c port=SWD -d $< 0x08000000 -rst

size: $(ELF)
	$(SIZE) --format=berkeley $(ELF)

clean:
	$(MAKE) -C $(DRIVERS_DIR) clean || true
	rm -rf $(BUILD_DIR)

-include $(APP_OBJS:.o=.d)
