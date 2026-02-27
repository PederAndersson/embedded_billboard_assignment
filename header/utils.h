#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "clientManager.h"


void odd_even(uint32_t seconds, client *c, uint8_t maxLen);
void string_copy(char* strSrc, char* strDest, uint8_t maxLen);
void swedish_parser(char *text);
void lcd_print(uint8_t row, char *str);
void lcd_print_scroll(uint8_t row, char*str);
void lcd_print_text(uint8_t row, char *str);
void lcd_print_blink(uint8_t row, char *str);
void srand_init();

#endif