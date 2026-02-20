#include <avr/io.h>
#include <avr/interrupt.h>
#include "systemTick.h"
#include "lcd.h"
#include "clientManager.h"
#include "utils.h"

#define ms_second 1000
#define switch_mode 3000
#define switch_effect 1000

client test = {
    .client_name = {"Svarte Petter"},
    .billboards = {{"hejsan"}, {"tjofräs"}, {"tomte"}},
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
    uint32_t previousSeconds = 0;
    uint32_t seconds = 0;
    uint32_t effectTime = 0;
    uint32_t modeTime = 0;
    //manager_config(&test, &mgr);
    manager_config(&test2, &mgr);
    mode = ODD_EVEN;
    effect = PLAIN_TEXT;
    
    sei();

    while(1) {
        uint32_t current = millis();
        if (current - previousSeconds >= ms_second){ seconds++; previousSeconds = current;}
        // if (current - modeTime >= switch_mode){ next_mode(&mode); modeTime = current;}
        // if (mode == TEXT && current - effectTime >= switch_effect){
        //     next_effect(&effect);
        //     effect_output(effect, &mgr);
        //     effectTime = current;
        //     }
        if (mode == ODD_EVEN){
            odd_even(seconds, &test2, ADD_LENGTH);
        }
    }

    return 0;
}
