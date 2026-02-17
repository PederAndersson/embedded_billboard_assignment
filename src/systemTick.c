#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "systemTick.h"

static volatile uint32_t system_tick = 0;

void Timer0_init(void) {
    TCNT0 = 0;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS00) | (1 << CS01);
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);
    
}

uint32_t millis(void) {   
    
    uint32_t now = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        now = system_tick;
    }
    return now;
}

ISR(TIMER0_COMPA_vect) {
    system_tick++;
}

