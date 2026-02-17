#include <avr/io.h>
#include <avr/interrupt.h>
#include "systemTick.h"
#include "lcd.h"

int main(void) {
    // Initialize timer for system ticks
    Timer0_init();
    
    // Initialize LCD display
    lcd_init();
    
    // Enable global interrupts
    sei();
    
    // Main loop
    while(1) {
        // Your code here
    }
    
    return 0;
}
