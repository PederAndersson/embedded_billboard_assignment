#include <avr/io.h>
#include <avr/interrupt.h>
#include "systemTick.h"
#include "lcd.h"
#include "clientManager.h"
#include "utils.h"
#include <stdint.h>

#define second_ms 1000
#define switch_mode 12000
#define switch_effect 4000

client test = {
    .client_name = {"Svarte Petter"},
    .billboards = {{"hejsan"}, {"åäöÅÄÖ"}, {"tomte"}},
    .price = 5000
};

client test2 = {
    .client_name = {"IoT billboard"},
    .billboards = { {"reklam"}, {"reklam2"}},
    .price = 1000
};


client_manager mgr;
enum Mode mode;
enum Effect effect;

int main(void) {

    Timer0_init(); 
    lcd_init();
    uint32_t previous_seconds = 0;
    uint32_t seconds = 0;
    uint32_t next_effect_switch = 0;
    uint32_t mode_time = 0;
    uint32_t effect_time = 0;
    uint32_t effect_duration = 0;

    manager_config(&test, &mgr);
    manager_config(&test2, &mgr);
    mode = TEXT;
    effect = PLAIN_TEXT;
    
    sei();

    while(1) {
    uint32_t current_time = millis(); 
        lcd_printf(1, mgr.client_list[0]->billboards[1]);
    }

    return 0;
}
