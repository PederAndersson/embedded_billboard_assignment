#include <avr/io.h>
#include <avr/interrupt.h>
#include "systemTick.h"
#include "lcd.h"
#include "clientManager.h"
#include "util/delay.h"
#include "utils.h"
#include <stdint.h>
#include <avr/delay.h>

#define second_ms 1000
#define switch_mode 12000
#define switch_effect 4000

char hej[] = "hejhej";
client_manager mgr;
enum Mode mode;
enum Effect effect;
int main(void) {

    Timer0_init(); 
    lcd_init();
    _delay_ms(100);
    uint32_t previous_seconds = 0;
    uint32_t seconds = 0;
    uint32_t next_effect_switch = 0;
    uint32_t mode_time = 0;
    uint32_t effect_time = 0;
    uint32_t effect_duration = 0;
    client test = parse_client_info(0);
    mode = TEXT;
    effect = PLAIN_TEXT;

    sei();

    while(1) {
    uint32_t current_time = millis(); 
    if(current_time - previous_seconds >= switch_effect){
        lcd_print(0, test.client_name);
        lcd_print(1, test.billboards[0]);
        _delay_ms(500);
        previous_seconds = current_time;
    }
        lcd_print(0, test.billboards[1]);
        lcd_print(1, test.billboards[2]);
        
    }
    
    return 0;
}