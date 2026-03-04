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
#include "billboardManager.h"



uint32_t minute = 0;
uint32_t current_time = 0;


client_manager mgr;
player play;

int main(void) {
    Timer0_init();
    lcd_init();
    srand_init();
    add_clients(&mgr);
    player_init(&play,&mgr, current_time);
    sei();
    while(1) {
    current_time = millis();
    run(&mgr,&play, current_time);
    }
    
    return 0;
}