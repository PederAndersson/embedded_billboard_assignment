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


client_manager mgr;
mode m;
effect e;

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
    
    m = TEXT;
    e = PLAIN_TEXT;
  
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