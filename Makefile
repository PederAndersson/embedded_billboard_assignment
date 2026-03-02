# Makefile for ATmega328P

# Compiler and tools
CC = avr-gcc
OBJCOPY = avr-objcopy
MCU := atmega328p
AVRDUDE = avrdude

CFLAGS_COMMON = -Wall -Wextra -Iheader/ -DF_CPU=16000000UL -mmcu=${MCU} -MMD -MP
CFLAGS_RELEASE = -Os
CFLAGS_DEBUG = -Og -g3 -fno-inline
CFLAGS = $(CFLAGS_RELEASE) $(CFLAGS_COMMON)
LDFLAGS = -mmcu=${MCU}

# Target settings
TARGET = main
BUILD_DIR = build
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200


# Source files
SRCS = src/main.c src/systemTick.c src/lcd.c src/clientManager.c src/utils.c src/clients.c src/billboardManager.c
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))

# Targets
all: $(BUILD_DIR)/$(TARGET).hex

debug: DEBUG=1
debug: CFLAGS = $(CFLAGS_DEBUG) $(CFLAGS_COMMON)
debug: $(BUILD_DIR)/$(TARGET).elf
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(BUILD_DIR)/$(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all debug upload clean
-include $(OBJS:.o=.d)