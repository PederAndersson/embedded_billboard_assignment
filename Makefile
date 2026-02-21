# Makefile for ATmega328P

# Compiler and tools
CC = avr-gcc
OBJCOPY = avr-objcopy
MCU := atmega328p
AVRDUDE = avrdude
CFLAGS = -mmcu=atmega328p -Os -Wall -Wextra -Iheader/ -DF_CPU=16000000UL -mmcu=${MCU}
LDFLAGS = -mmcu=atmega328p

# Target settings
TARGET = main
MCU = atmega328p
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200

# Source files
SRCS = src/main.c src/systemTick.c src/lcd.c src/clientManager.c src/utils.c
OBJS = $(SRCS:.c=.o)

# Targets
all: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex

.PHONY: all upload clean