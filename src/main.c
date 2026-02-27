#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "systemTick.h"
#include "lcd.h"
#include "clientManager.h"
#include "util/delay.h"
#include "utils.h"
#include "clients.h"
#include <stdint.h>

#define second_ms 250
#define switch_mode 12000
#define switch_effect 4000

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU / (8UL * BAUD)) - 1)

static inline void serial_init(void) {
    UCSR0A = (1 << U2X0); // double speed for better 115200 accuracy
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)(UBRR_VALUE & 0xFF);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8N1
}
static inline void serial_write_byte(uint8_t b) {
    while (!(UCSR0A & (1 << UDRE0))) {
        // wait for transmit buffer
    }
    UDR0 = b;
}

static inline void serial_write_str(const char *s) {
    while (*s) {
        serial_write_byte((uint8_t)*s++);
    }
}

static inline void serial_write_u32(uint32_t v) {
    char buf[11]; // max 10 digits + NUL for uint32_t
    uint8_t i = 0;
    if (v == 0) {
        serial_write_byte('0');
        return;
    }
    while (v > 0 && i < sizeof(buf) - 1) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }
    while (i > 0) {
        serial_write_byte((uint8_t)buf[--i]);
    }
}

static inline void serial_write_u8(uint8_t v) {
    char buf[11]; // max 10 digits + NUL for uint32_t
    uint8_t i = 0;
    if (v == 0) {
        serial_write_byte('0');
        return;
    }
    while (v > 0 && i < sizeof(buf) - 1) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }
    while (i > 0) {
        serial_write_byte((uint8_t)buf[--i]);
    }
}



client_manager mgr;
enum Mode mode;
enum Effect effect;

int main(void) {
    Timer0_init(); 
    lcd_init();
    srand_init();
    
    uint32_t seed = 0;

    uint32_t previous_seconds = 0;
    uint32_t seconds = 0;
    uint32_t next_effect_switch = 0;
    uint32_t mode_time = 1;
    uint32_t effect_time = 0;
    uint32_t effect_duration = 0;

    add_clients(&mgr);
    
    mode = TEXT;
    effect = PLAIN_TEXT;
  
    sei();

    while(1) {
    uint32_t current_time = millis(); 
        if (current_time - seconds >= 2500){
            lcd_print_blink(1, mgr.client_list[0].billboards[1]);
            seconds = current_time;
        }
    }
    
    return 0;
}