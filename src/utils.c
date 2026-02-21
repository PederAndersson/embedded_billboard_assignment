#include <stdint.h>
#include "utils.h"
#include "lcd.h"

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

