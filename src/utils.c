#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
#include "lcd.h"
#include "util/delay.h"


void odd_even(uint32_t seconds, client *client, uint8_t maxLen){
    lcd_set_cursor(0, 1);
    char str[maxLen + 1];
    if (seconds%2){
        string_copy(&client->billboards[0][0], str, maxLen);
    }else {
        string_copy(&client->billboards[1][0], str, maxLen);
    }
    for (uint8_t i = 0; i < maxLen; i++){
        lcd_write(str[i]);
    }
}

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

void swedish_parser(char *text) {
    char *src = text;
    char *dst = text;

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
    static uint8_t offset = 0;
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
       offset = 0;
       return;
    }
    
    for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
        uint8_t idx = (offset + i) % line_len;
        lcd_write(line[idx]);
    }
    offset = (offset + 1) % line_len;
}

void lcd_print_text(uint8_t row, char *str){
    static uint8_t offset = 0;
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
        offset = 0;
        return;
    }
    static uint8_t second_half_idx = 0;
    if (offset > 0){
        uint8_t idx = 0;
        for (uint8_t i = second_half_idx; line[i] != '\0'; i++){
            lcd_write(line[i]);
            idx++;
        
        }
        if (idx < LCD_COL_COUNT){
            for (uint8_t i = idx; i < LCD_COL_COUNT; i++){
                lcd_write(' ');
            }
            
        }
        second_half_idx = 0;
        offset = 0;
    }else if (offset == 0) {

        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            uint8_t idx = (offset + i);
            lcd_write(line[idx]);
            second_half_idx++;
        }
        offset = 1;
    }
    
}

void lcd_print_blink(uint8_t row, char *str){
    static uint8_t offset = 0;
    static uint8_t blink = 0;
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

    if (blink == 1){
        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            lcd_write(' ');
        }
        blink = 0;
        return;
    }
    
    if (str_len <= LCD_COL_COUNT && blink == 0){
        
        for (uint8_t i = 0; i < str_len; i++){
            lcd_write(line[i]);
        }
        blink = 1; 
        offset = 0;  
        return;
    }
    static uint8_t second_half_idx = 0;
    if (offset > 0 && blink == 0){
        uint8_t idx = 0;
        for (uint8_t i = second_half_idx; line[i] != '\0'; i++){
            lcd_write(line[i]);
            idx++;
        }
        if (idx < LCD_COL_COUNT){
            for (uint8_t i = idx; i < LCD_COL_COUNT; i++){
                lcd_write( ' ');
            }
        }
        blink = 1;
        offset = 0;
        second_half_idx = 0;
    }
    else if (offset == 0 && blink == 0){
        for (uint8_t i = 0; i < LCD_COL_COUNT; i++){
            lcd_write(line[i]);
            second_half_idx++;
        }
        offset = 1;
        blink = 1;
    }
}


static uint8_t read_adc(){

    DIDR0 |= (1 << ADC0D);
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)){}
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)){}
    uint16_t v = ADC;
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
