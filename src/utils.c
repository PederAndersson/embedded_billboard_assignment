#include <stdint.h>
#include "utils.h"
#include "lcd.h"

void odd_even(uint32_t seconds, client *client, uint8_t maxLen){
    lcd_set_cursor(0, 1);
    char str[maxLen + 1];
    if (seconds%2){
        string_copy(client->billboards[0][0], str, maxLen);
    }else {
        string_copy(client->billboards[1][0], str, maxLen);
    }
    for (uint8_t i = 0; i < maxLen; i++){
        lcd_write(str[i]);
    }
}

void string_copy(char* strSrc, char* strDest, uint8_t maxLen){
    uint8_t i = 0;
    
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

