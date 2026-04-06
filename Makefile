# =====================================================================
#  Makefile for FL-DCD-7224-4 Digital Light Source Controller
#  STM32G030C8T6 - ARM Cortex-M0+
# =====================================================================
#
#  Prerequisites:
#    - arm-none-eabi-gcc toolchain
#
#  Usage:
#    make            - Build firmware
#    make clean      - Clean build artifacts
#    make flash      - Flash via OpenOCD + ST-Link
#    make size       - Show firmware size
#
# =====================================================================

# ---- Toolchain ----
PREFIX = arm-none-eabi-
CC     = $(PREFIX)gcc
AS     = $(PREFIX)gcc -x assembler-with-cpp
CP     = $(PREFIX)objcopy
SZ     = $(PREFIX)size
HEX    = $(CP) -O ihex
BIN    = $(CP) -O binary -S

# ---- Project ----
TARGET = fl-dcd-7224
BUILD  = build

# ---- Source Files ----
C_SOURCES = \
	Core/Src/main.c \
	Core/Src/tim.c \
	Core/Src/usart.c \
	Core/Src/gpio.c \
	Core/Src/ch455g.c \
	Core/Src/buttons.c \
	Core/Src/params.c \
	Core/Src/trigger.c \
	Core/Src/overcurrent.c \
	Core/Src/stm32g0xx_it.c \
	Core/Src/system_stm32g0xx.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_uart.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_adc.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c \
	Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c

# Startup assembly
ASM_SOURCES = startup_stm32g030xx.s

# ---- Include Paths ----
C_INCLUDES = \
	-ICore/Inc \
	-IDrivers/CMSIS/Include \
	-IDrivers/CMSIS/Device/ST/STM32G0xx/Include \
	-IDrivers/STM32G0xx_HAL_Driver/Inc

# ---- MCU Flags ----
CPU    = -mcpu=cortex-m0plus
FPU    =
FLOAT  =

MCU = $(CPU) -mthumb $(FPU) $(FLOAT)

# ---- Compiler Flags ----
CFLAGS = $(MCU) -std=gnu11 -Wall -Wextra
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -DSTM32G030xx
CFLAGS += -O2 -g
CFLAGS += $(C_INCLUDES)

ASFLAGS = $(MCU) -Wall -fdata-sections -ffunction-sections

# ---- Linker Flags ----
LDSCRIPT = STM32G030C8Tx_FLASH.ld

LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map=$(BUILD)/$(TARGET).map,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -lc -lm -lnosys

# ---- Build Rules ----
OBJECTS  = $(addprefix $(BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).hex $(BUILD)/$(TARGET).bin
	$(SZ) $(BUILD)/$(TARGET).elf

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s | $(BUILD)
	$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(HEX) $< $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(BIN) $< $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

flash: $(BUILD)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg \
		-c "program $< verify reset exit"

size: $(BUILD)/$(TARGET).elf
	$(SZ) $<

.PHONY: all clean flash size
