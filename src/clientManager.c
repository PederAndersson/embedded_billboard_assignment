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

void effect_output(enum Effect e, client_manager *mgr){
    switch(e){
        case PLAIN_TEXT:{}
        
        case SCROLL: {}

        case BLINK: {}
    }
}