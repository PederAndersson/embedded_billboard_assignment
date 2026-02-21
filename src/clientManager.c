#include <stdint.h>
#include "clientManager.h"
#include "lcd.h"

static uint8_t size = 0;

static uint32_t total_price(client *c, client_manager * mgr){
        return mgr->total_income += c->price;
}

void manager_config(client *c, client_manager* mgr){
    
    mgr->client_list[size] = c;
    mgr->previous_client = *c;
    mgr->total_income = total_price(c, mgr);
    if (size < CLIENTS){ size++;}

}

void next_mode(enum Mode *m){
    if (*m == TEXT){
        *m = ODD_EVEN;
    }else if (*m == ODD_EVEN){
        *m = TEXT;
    }
}

void next_effect(enum Effect *e){
    switch (*e){
        case PLAIN_TEXT: {*e = SCROLL; break;}
        case SCROLL: {*e = BLINK; break;}
        case BLINK: {*e = PLAIN_TEXT; break;}
    }
}

void effect_output(enum Effect e, client_manager *mgr, uint32_t time){
    uint32_t previous = 0;

    switch(e){
        case PLAIN_TEXT:{
            lcd_set_cursor(0, 0);
            lcd_puts(mgr->client_list[0]->client_name);
            lcd_set_cursor(0, 1);
            lcd_puts(mgr->client_list[0]->billboards[0]);
            break;
        }
        case SCROLL: {
            lcd_set_cursor(0, 0);
            lcd_puts(mgr->client_list[0]->client_name);
            lcd_set_cursor(0, 1);
            lcd_stringCopy(1, mgr->client_list[0]->billboards[1]);
            break;
        }
        case BLINK: {
            lcd_set_cursor(0, 0);
            lcd_puts(mgr->client_list[0]->client_name);
            lcd_stringCopy(0, mgr->client_list[0]->billboards[2]);
            break;
        }
    }
}