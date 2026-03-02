#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
#include "lcd.h"
#include "util/delay.h"

typedef struct offset_variables{
    uint8_t offset;
    uint8_t blink;
    uint8_t second_half_idx;
}variables;

static variables v = {
    .offset = 0,
    .blink = 0,
    .second_half_idx = 0
};

void string_copy(char* strSrc, char* strDest, uint8_t maxLen){
    uint8_t i = 0;
    swedish_parser(strSrc);
    while (strSrc[i] != '\0' && i < maxLen){
        strDest[i] = strSrc[i];
        i++;
    }
    while(i < maxLen){
        strDest[i] = ' ';
        i++;
    }
    strDest[maxLen] = '\0';
}

void swedish_parser(char *str) {
    char *src = str;
    char *dst = str;

    // Custom LCD glyph slots loaded in lcd_init (avoid slot 0 because NUL in C strings)
    const char lcd_aa_lower = 1;
    const char lcd_ae_lower = 2;
    const char lcd_oe_lower = 3;
    const char lcd_aa_upper = 4;
    const char lcd_ae_upper = 5;
    const char lcd_oe_upper = 6;

    while (*src) {
        // Check for UTF-8 encoded Swedish characters (0xC3 xx)
        if ((unsigned char)src[0] == 0xC3) {
            switch ((unsigned char)src[1]) {
                case 0xA5:  // å
                    *dst++ = lcd_aa_lower;
                    src += 2;
                    continue;

                case 0xA4:  // ä
                    *dst++ = lcd_ae_lower;
                    src += 2;
                    continue;

                case 0xB6:  // ö
                    *dst++ = lcd_oe_lower;
                    src += 2;
                    continue;

                // uppercase
                case 0x85:  // Å
                    *dst++ = lcd_aa_upper;
                    src += 2;
                    continue;

                case 0x84:  // Ä
                    *dst++ = lcd_ae_upper;
                    src += 2;
                    continue;

                case 0x96:  // Ö
                    *dst++ = lcd_oe_upper;
                    src += 2;
                    continue;
            }
        }

        // Normal character copy
        *dst++ = *src++;
    }
    *dst = '\0';
}

void lcd_print(uint8_t row, char *str){
    char line[LCD_COL_COUNT + 1];
    uint8_t i = 0;

    while (str[i] != '\0' && i < LCD_COL_COUNT) {
        line[i] = str[i];
        i++;
    }
    line[i] = '\0';

    swedish_parser(line);

    lcd_set_cursor(0, row);
    for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
        if (line[i] != '\0') {
            lcd_write(line[i]);
        } else {
            lcd_write(' ');
        }
    }
}

void lcd_print_scroll(uint8_t row, char *str){
    uint8_t i = 0;
    uint8_t str_len = 0;
    while(str[str_len] != '\0'){
        str_len++;
    }
    char line[str_len + 1];
    while (i < str_len ){
        line[i] = str[i];
        i++;
    }
    line[i] = '\0';
    swedish_parser(line);
    uint8_t line_len = 0;
    while (line[line_len] != '\0'){
        line_len++;
    }
    lcd_set_cursor(0, row);
    if (line_len <= LCD_COL_COUNT){
       for (uint8_t i = 0; i < line_len; i++){lcd_write(line[i]);}
       for (uint8_t i = line_len; i < LCD_COL_COUNT; i++){lcd_write(' ');}
       v.offset = 0;
       return;
    }
    
    for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
        uint8_t idx = (v.offset + i) % line_len;
        lcd_write(line[idx]);
    }
    v.offset = (v.offset + 1) % line_len;
}

void lcd_print_text(uint8_t row, char *str){
    uint8_t str_len = 0;
    uint8_t i = 0;

    while (str[str_len] != '\0'){
        str_len++;
    }
    char line[str_len+1];
    while (i < str_len){
        line[i] = str[i];
        i++;
    }
    line[i] = '\0';
    swedish_parser(line);
    uint8_t line_len = 0;
    while (line[line_len] != '\0'){
        line_len++;
    }
    lcd_set_cursor(0, row);
    if (line_len <= LCD_COL_COUNT){
        for (uint8_t i = 0; i < line_len; i++ ){lcd_write(line[i]);}
        for (uint8_t i = line_len; i < LCD_COL_COUNT; i++){lcd_write(' ');} 
        v.offset = 0;
        return;
    }
    if (v.offset > 0){
        uint8_t idx = 0;
        for (uint8_t i = v.second_half_idx; line[i] != '\0'; i++){
            lcd_write(line[i]);
            idx++;
        
        }
        if (idx < LCD_COL_COUNT){
            for (uint8_t i = idx; i < LCD_COL_COUNT; i++){
                lcd_write(' ');
            }
            
        }
        v.second_half_idx = 0;
        v.offset = 0;
    }else if (v.offset == 0) {

        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            uint8_t idx = (v.offset + i);
            lcd_write(line[idx]);
            v.second_half_idx++;
        }
        v.offset = 1;
    }
    
}

void lcd_print_blink(uint8_t row, char *str){
    uint8_t str_len = 0;
    uint8_t i = 0;

    while (str[str_len] != '\0'){
        str_len++;
    }
    char line[str_len + 1];
    while (i < str_len){
        line[i] = str[i];
        i++;
    }
    line[i] = '\0';
    swedish_parser(line);
    lcd_set_cursor(0, row);

    if (v.blink == 1){
        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            lcd_write(' ');
        }
        v.blink = 0;
        return;
    }
    
    if (str_len <= LCD_COL_COUNT && v.blink == 0){
        
        for (uint8_t i = 0; i < str_len; i++){
            lcd_write(line[i]);
        }
        v.blink = 1; 
        v.offset = 0;  
        return;
    }
    if (v.offset > 0 && v.blink == 0){
        uint8_t idx = 0;
        for (uint8_t i = v.second_half_idx; line[i] != '\0'; i++){
            lcd_write(line[i]);
            idx++;
        }
        if (idx < LCD_COL_COUNT){
            for (uint8_t i = idx; i < LCD_COL_COUNT; i++){
                lcd_write( ' ');
            }
        }
        v.blink = 1;
        v.offset = 0;
        v.second_half_idx = 0;
    }
    else if (v.offset == 0 && v.blink == 0){
        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            lcd_write(line[i]);
            v.second_half_idx++;
        }
        v.offset = 1;
        v.blink = 1;
    }
}
 
static void reset(){
    v = (variables){0};
}

void reset_offset(){
    reset();
}

static uint8_t read_adc(){
    // DIDR0 - Digital Input Disable Register 0
    // Disables digital input buffer on ADC0 pin to reduce power consumption
    // ADC0D bit enables the ADC for analog measurements
    DIDR0 |= (1 << ADC0D);
    
    // ADMUX - ADC Multiplexer Selection Register
    // REFS0 = 1 sets reference voltage to AVCC (typically 5V), needed for conversion
    // Default channel (ADC0) is selected when all bits are 0
    ADMUX = (1 << REFS0);
    
    // ADCSRA - ADC Control and Status Register A
    // ADEN = 1 enables the ADC module
    // ADPS2:ADPS0 = 111 sets prescaler to 128 (ADPS0|ADPS1|ADPS2)
    //   - Prescaler divides CPU clock to get ADC clock (16MHz/128 = 125kHz)
    //   - ADC requires clock between 50-200kHz for accurate conversion
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Run ADC twice: first conversion discarded (may have stale result),
    // second conversion is the actual value used for seeding
    for (uint8_t i = 0; i < 2; i ++){
        // ADSC - ADC Start Conversion bit
        // Setting ADSC to 1 starts a new ADC conversion
        ADCSRA |= (1 << ADSC);
        
        // Wait until conversion completes
        // ADSC becomes 0 when conversion is done
        while (ADCSRA & (1 << ADSC)){}
    }
    
    // ADC - ADC Data Register
    // Contains the 10-bit result of the analog conversion (0-1023)
    // MSB is in bit 9, LSB is in bit 0
    uint16_t v = ADC;
    
    // Extract the least significant bit for random seed contribution
    // Uses noise from ADC's least significant bit for entropy
    uint8_t bit = v & 1;
    return bit;
}

void srand_init(){
    uint32_t seed = 0;
    for (uint8_t i = 0; i < 32; i++){
        seed = (seed << 1) | read_adc();
    }
    srand(seed);
}
