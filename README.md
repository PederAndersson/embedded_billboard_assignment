# LCD Billboard - ATmega328P Advertising Display

An embedded advertising billboard system that cycles through client advertisements on a 16x2 LCD display. Clients pay for display time, and the system uses weighted random selection to give higher-paying clients more screen time.

## Hardware

- **Microcontroller:** ATmega328P (Arduino Uno)
- **Display:** HD44780-compatible 16x2 LCD in 4-bit mode
- **Clock:** 16 MHz external crystal

### Pin Configuration

| LCD Pin | ATmega328P Pin (PORTD) |
|---------|------------------------|
| RS      | PD0                    |
| EN      | PD1                    |
| D4      | PD4                    |
| D5      | PD5                    |
| D6      | PD6                    |
| D7      | PD7                    |
| RW      | GND                    |

A circuit schematic is available in `documents/lcd_billboard.kicad_sch`.

### Component Choices

The **HD44780 LCD** was chosen because it is simple to interface in 4-bit mode (only 6 GPIO pins), widely available, and has low power consumption. 4-bit mode was used over 8-bit mode to save GPIO pins for potential future expansion.

The **ATmega328P** provides enough flash (32 KB) to store client data in PROGMEM, 2 KB SRAM for runtime state, and hardware timers for accurate millisecond timing. Its AVR architecture is well-supported by avr-gcc and avrdude.

**ADC0** is used as an entropy source for `srand()`. The least significant bit of 32 ADC readings is collected to build a 32-bit seed, giving reasonably random client selection on each power cycle.

## Software Architecture

```
main.c                 System initialization and main loop
├── systemTick.c       Timer0 ISR providing millis() timebase
├── clientManager.c    CSV parser and weighted client selection
├── billboardManager.c State machine controlling display transitions
├── utils.c            Text effects (scroll, blink, text) and Swedish character support
├── lcd.c              HD44780 LCD driver (4-bit mode)
└── clients.c          Client database stored in PROGMEM
```

### Main Loop

The system uses a non-blocking, timer-driven main loop. `millis()` provides a 1 ms timebase via Timer0 in CTC mode (prescaler 64, compare match at 249). The main loop calls `run()` on every iteration, which checks elapsed time to decide what to do — no `delay()` calls block execution.

### State Machine

The billboard manager uses a two-state machine:

- **SHOW_COMMERCIAL** — Displays the current client's name (row 0) and advertisement (row 1) with the configured text effect. Transitions to SWITCH after `BILLBOARDS_MS` (20 seconds).
- **SWITCH** — Clears the display for `SWITCH_MS` (2.5 seconds), then selects the next client and transitions back to SHOW_COMMERCIAL.

### Client Selection

Clients are selected using **weighted random selection** based on their price. A client's probability of being shown is proportional to their price relative to the total. The previously displayed client is excluded from the next selection to avoid consecutive repeats.

Some clients use an **odd/even minute** display option instead, which alternates between two different advertisements based on whether the current minute count is odd or even.

### Client Data Format

Client data is stored as CSV strings in PROGMEM (`clients.c`) and parsed at startup. The format is:

```
name, billboard1, billboard2, billboard3, price, number_of_ads, id, effect1, effect2, effect3, selection_option
```

- **Effects:** 0 = static text, 1 = scrolling, 2 = blinking
- **Selection option:** 0 = weighted random, 1 = odd/even minute

The parser uses a state machine (`parse_state`) to walk through each CSV field. Storing client data in PROGMEM keeps it in flash memory, preserving the limited 2 KB SRAM for runtime use.

### Text Effects

All text effects operate non-blocking using elapsed-time checks:

- **Static text** — Displays text on the LCD. If the text is longer than 16 characters, it alternates between the first and second half every `TEXT_SWITCH` (2 seconds).
- **Scrolling** — Wraps the text in a circular buffer, advancing one character every `SCROLL_MS` (300 ms).
- **Blinking** — Alternates between showing and hiding the text, with configurable on/off durations (`BLINK_ON` = 1500 ms, `BLINK_OFF` = 1000 ms).

### Swedish Character Support

The HD44780 LCD does not have Swedish characters (å, ä, ö) in its character ROM. Custom glyphs are loaded into the LCD's CGRAM at initialization (slots 1-6, avoiding slot 0 which conflicts with C null terminators). A UTF-8 parser in `swedish_parser()` detects the two-byte sequences (0xC3 + second byte) and replaces them with the corresponding CGRAM slot index.

### Memory Management

The ATmega328P has 2 KB SRAM. Key memory decisions:

- **`client_manager` is `static`** in `main()` — placed in `.bss` (statically allocated) rather than on the stack, since the struct is approximately 1.1 KB.
- **Client strings are in PROGMEM** — the raw CSV data lives in flash, only parsed into RAM once at startup.
- **`millis()` uses `ATOMIC_BLOCK`** — ensures the 32-bit `system_tick` is read atomically on the 8-bit AVR, preventing torn reads when the ISR fires mid-access.

### Timing

| Parameter          | Value     | Purpose                          |
|--------------------|-----------|----------------------------------|
| `BILLBOARDS_MS`    | 20 000 ms | Duration each client is shown    |
| `SWITCH_MS`        | 2 500 ms  | Blank screen between clients     |
| `SCROLL_MS`        | 300 ms    | Scroll speed per character       |
| `BLINK_ON`         | 1 500 ms  | Text visible duration            |
| `BLINK_OFF`        | 1 000 ms  | Text hidden duration             |
| `TEXT_SWITCH`       | 2 000 ms  | Page flip for long static text   |
| `MINUTE_MS`        | 60 000 ms | Minute counter tick              |

## Building and Uploading

### Prerequisites

- `avr-gcc` toolchain
- `avrdude` for flashing

### Build

```bash
make            # Release build (size-optimized with -Os)
make debug      # Debug build (with -Og -g3 for debugging)
```

### Upload

```bash
make upload     # Flash via Arduino bootloader on /dev/ttyACM0
```

To use a different serial port, override the `PORT` variable:

```bash
make upload PORT=/dev/ttyUSB0
```

### Clean

```bash
make clean
```

## Possible Improvements

- **Unit testing** — The parser and state machine logic could be tested on a host machine by abstracting the hardware-dependent parts behind an interface.
- **Decouple `utils.c`** — The text effect functions currently depend on `billboardManager.h` through the `player` struct. Passing primitive timing values instead would remove this coupling.
- **Dynamic client data** — Client data could be received over UART or I2C instead of being compiled into PROGMEM, enabling updates without reflashing.
- **EEPROM persistence** — The minute counter and last-displayed client could be saved to EEPROM to survive power cycles.
- **read client info from flash** - Instead of copying the entire client data from the flash memory to .bss of the SRAM just copy relevant metadata for selecting the next client and then just reading the neccessary data from the flash memory each time.
