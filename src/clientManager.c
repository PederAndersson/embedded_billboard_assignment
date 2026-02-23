#include "clientManager.h"
#include "avr/pgmspace.h"
#include "lcd.h"
#include "clients.h"
#include "utils.h"

static uint8_t size = 0;
static uint32_t scroll_ms = SCROLL_MS;
static uint32_t blink_ms = BLINK_MS;
static char parse_buffer[PARSE_BUFFER+1];
static const uint8_t number_ads = 2;


static uint32_t total_price(client *c, client_manager * mgr){
        return mgr->total_income += c->price;
}

void next_mode(enum Mode *m){
    if (*m == TEXT){
        *m = ODD_EVEN;
    }else if (*m == ODD_EVEN){
        *m = TEXT;
    }
}

void next_effect(enum Effect *e, client_manager* mgr){
    switch (*e){
        case PLAIN_TEXT: {
            lcd_stringCopy(1, mgr->client_list[0].billboards[1]);
            *e = SCROLL;
            break;}
        case SCROLL: {
            lcd_stringCopy(0, mgr->client_list[0].billboards[2]);
            *e = BLINK;
            break;}
        case BLINK: {*e = PLAIN_TEXT; break;}
    }
}

void effect_output(enum Effect e, client_manager *mgr, uint32_t *out){

    switch(e){
        case PLAIN_TEXT:{
            lcd_printf(0, mgr->client_list[0].client_name);
            lcd_printf(1, mgr->client_list[0].billboards[0]);
            *out = 0;  
            break;
        }
        case SCROLL: {
            lcd_printf(0,mgr->client_list[0].client_name);
            lcd_scroll_left_row(1);
            *out = scroll_ms;
            break;
        }
        case BLINK: {
            lcd_printf(0,mgr->client_list[0].client_name);
            lcd_blink_row(1);
            *out = blink_ms;
            break;
        }
    }

}

client parse_client_info(uint8_t row){
    client temp;
    uint8_t i = 0;

    temp.client_name[0] = '\0';
    for (uint8_t i = 0; i < NUMBER_ADS; i++){temp.billboards[i][0] = '\0';}
    temp.price = 0;

    const char* row_ptr = (const char*)pgm_read_ptr(&list[row]);
    while (i < PARSE_BUFFER){
        char c = (char)pgm_read_byte(row_ptr+i);
        if (c == '\0'){break;}
        parse_buffer[i] = c;
        i++;
    }
    parse_buffer[i] = '\0';

    parse_state st = S_CLIENT;

    uint8_t ad_idx = 0;
    uint8_t cpy_idx = 0;
    uint32_t price = 0;

    char *dest = temp.client_name;
    uint16_t dest_max = sizeof(temp.client_name);

    for (uint16_t k = 0; ; k++){
        char c = parse_buffer[k];

        if (c == ',' || c == '\0'){
            if (st != S_PRICE){
                if (dest_max > 0){dest[cpy_idx] = '\0';}
            }

            if (st == S_CLIENT){
                st = S_BILLBOARD;
                ad_idx = 0;
                dest = temp.billboards[0];
                dest_max = sizeof(temp.billboards[0]);
            }else if (st == S_BILLBOARD){
                if(ad_idx < NUMBER_ADS - 1){
                    ad_idx++;
                    dest = temp.billboards[ad_idx];
                    dest_max = sizeof(temp.billboards[ad_idx]);
                } else {
                    st = S_PRICE;
                }
            }else if (st == S_PRICE){
                st = S_DONE;
            }
            cpy_idx = 0;

            if (c == '\0' || st == S_DONE){break;}
            continue;
        }

        if (st == S_PRICE){
            if (c >= '0' && c <= '9'){
                price = price * 10u + (uint32_t)(c - '0');
            } else{

            }
        }else {
            if (cpy_idx == 0 && (c == ' ' || c == '\t')){
                continue;
            }

            if (cpy_idx < (uint8_t)(dest_max-1)){
                dest[cpy_idx++] = c;
            }
        }
    }

    temp.price = price;
    return temp;

}

void add_clients(client_manager *mgr){

    mgr->total_income = 0;
    for (uint8_t i = 0; i < rows_count; i++){
        mgr->client_list[i] = parse_client_info(i);
        total_price(&mgr->client_list[i], mgr);
    }
}