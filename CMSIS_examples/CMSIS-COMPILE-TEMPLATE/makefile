TARGET  = firmware
BUILD   = build
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CMSIS = Drivers/CMSIS
CPU   = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

DEFS  = -DSTM32F411xE -DHSE_VALUE=25000000U
INCS  = -I$(CMSIS)/Core/Include -I$(CMSIS)/Device/ST/STM32F4xx/Include -Isrc

CFLAGS  = $(CPU) $(DEFS) $(INCS) -O2 -g3 -Wall -ffunction-sections -fdata-sections -MMD -MP
LDFLAGS = $(CPU) -TSTM32F411XE_FLASH.ld --specs=nano.specs --specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

SRCS = $(wildcard src/*.c)
ASMS = $(wildcard src/*.s)
OBJS = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS)) $(patsubst src/%.s,$(BUILD)/%.o,$(ASMS))

ELF = $(BUILD)/$(TARGET).elf
BIN = $(BUILD)/$(TARGET).bin

# Gravador: stlink (st-flash) ou openocd
FLASHER ?= stlink

# ---------- Alvos principais ----------
all: clean compile

compile: $(ELF) $(BIN)

flash: compile
ifeq ($(FLASHER),openocd)
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	        -c "program $(ELF) verify reset exit"
else
	st-flash --reset write $(BIN) 0x08000000
endif

clean:
	rm -rf $(BUILD)

# ---------- Regras de build ----------
$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: src/%.s | $(BUILD)
	$(CC) $(CPU) -x assembler-with-cpp -c $< -o $@

$(ELF): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(BUILD):
	mkdir -p $@

-include $(OBJS:.o=.d)
.PHONY: all compile flash clean