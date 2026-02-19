#include <stdint.h>
#include "utils.h"
#include "lcd.h"

void odd_even(uint32_t seconds, client *client){
    lcd_set_cursor(0, 1);
    if (seconds%2){
        for (uint8_t i = 0; i < ADD_LENGTH; i++){
            lcd_write(*client->billboards[0][i]);
        }
    }else {
        for (uint8_t i = 0; i < ADD_LENGTH; i++){ 
            lcd_write(*client->billboards[1][i]);
        }
    }
}

