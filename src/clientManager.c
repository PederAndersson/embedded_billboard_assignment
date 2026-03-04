#include <stdlib.h>
#include "clientManager.h"
#include "avr/pgmspace.h"
#include "lcd.h"
#include "clients.h"
#include "utils.h"

static char parse_buffer[PARSE_BUFFER+1];

static uint32_t total_price(client *c, client_manager * mgr){
        return mgr->total_income += c->price;
}

static client parse_client_info(uint8_t row){
    client client;
    uint8_t i = 0;

    client.client_name[0] = '\0';
    for (uint8_t i = 0; i <= NUMBER_ADS; i++){client.billboards[i].billboard[0] = '\0';}
    client.price = 0;
    client.client_id = 0;
    client.number_ads = 0;

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
    uint8_t effect_idx = 0;
    uint8_t cpy_idx = 0;
    uint32_t price = 0;
    uint8_t ads = 0;
    uint8_t id = 0;
    uint8_t selection_option = 0;
    uint8_t effect = 0;

    char *dest = client.client_name;
    uint16_t dest_max = sizeof(client.client_name);

    for (uint16_t k = 0; ; k++){
        char c = parse_buffer[k];

        if (c == ',' || c == '\0'){
            if (st == S_CLIENT || st == S_BILLBOARD){
                if (dest_max > 0){dest[cpy_idx] = '\0';}
            }

            if (st == S_CLIENT){
                st = S_BILLBOARD;
                dest = client.billboards[ad_idx].billboard;
                dest_max = sizeof(client.billboards[ad_idx].billboard);
            }else if (st == S_BILLBOARD){
                if(ad_idx < NUMBER_ADS){
                    ad_idx++;
                    dest = client.billboards[ad_idx].billboard;
                    dest_max = sizeof(client.billboards[ad_idx].billboard);
                } else {
                    st = S_PRICE;
                }
            }else if (st == S_PRICE){
                st = S_ADS;
            }else if(st == S_ADS){
                st = S_ID;
            }else if (st == S_ID){
                st = S_EFFECT;
            }else if (st == S_EFFECT){
                client.billboards[effect_idx].effect = effect;
                if (effect_idx < ads - 1){
                    effect_idx++;                
                }
                else {
                    st = S_SELECTION;
                }
            }else if (st == S_SELECTION){
                st = S_DONE;
            }
            cpy_idx = 0;

            if (c == '\0' || st == S_DONE){break;}
            continue;
        }

        if (st == S_PRICE){
            if (c >= '0' && c <= '9'){
                price = price * 10u + (uint32_t)(c - '0');
            } 
        }else if(st == S_ADS){
            if (c >= '0' && c <= '9'){
                ads = (uint8_t)(c - '0');
            }
        }else if (st == S_ID){
            if (c >= '0' && c <= '9'){
                id = (uint8_t)(c - '0');
            }
        }else if (st == S_EFFECT){
            if (c >= '0' && c <= '9'){
                effect = (uint8_t)(c - '0');
            }
        }else if (st == S_SELECTION){
             if (c >= '0' && c <= '9'){
                selection_option = (uint8_t)(c - '0');
            }
        }
        else {
            if (cpy_idx == 0 && (c == ' ' || c == '\t')){
                continue;
            }

            if (cpy_idx < (uint8_t)(dest_max-1)){
                dest[cpy_idx++] = c;
            }
        }
    }
    client.client_id = id;
    client.number_ads = ads;
    client.price = price;
    client.display_option = selection_option;
    return client;
}

static void manager_init(client_manager *mgr){
    mgr->intervals.billboard_duration = BILLBOARDS_MS;
    mgr->intervals.blink_off = BLINK_OFF;
    mgr->intervals.blink_on = BLINK_ON;
    mgr->intervals.scroll_tick = SCROLL_MS;
    mgr->intervals.text_switch = TEXT_SWITCH;
    mgr->intervals.minutes_tick = MINUTE_MS;
    mgr->intervals.switch_duration = SWITCH_MS;

}

void add_clients(client_manager *mgr){

    mgr->total_income = 0;
    for (uint8_t i = 0; i < rows_count; i++){
        mgr->client_list[i] = parse_client_info(i);
        total_price(&mgr->client_list[i], mgr);
    }
    mgr->current_client = &mgr->client_list[rows_count - 1];
    manager_init(mgr);
}


void next_client(client_manager *mgr){

    mgr->previous_client = mgr->current_client;
    uint32_t adjusted_total = mgr->total_income - mgr->previous_client->price;
    uint32_t client_slot_number = rand() % adjusted_total;
    uint32_t client_prices = 0;
    client* next_client = NULL;

    for (uint8_t i = 0; i < rows_count; i++){
        if (&mgr->client_list[i] != mgr->previous_client){
            client_prices += mgr->client_list[i].price;
            if (client_prices > client_slot_number){
                next_client = &mgr->client_list[i];
                break;
            }
        }
    }
    mgr->current_client = next_client;
}


