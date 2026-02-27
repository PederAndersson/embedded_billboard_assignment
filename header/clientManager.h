#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "lcd.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#define CLIENTNAME 50
#define ADD_SIZE 40
#define NUMBER_ADS 2
#define PARSE_BUFFER 254

#define CLIENTS 5
#define SCROLL_MS 200
#define BLINK_MS 350




typedef enum  {
    TEXT,
    ODD_EVEN
} mode;

typedef enum {
    PLAIN_TEXT,
    SCROLL,
    BLINK
}effect;

typedef enum {
    S_CLIENT,
    S_BILLBOARD,
    S_PRICE,
    S_ADS,
    S_DONE
}parse_state;

typedef struct {
    char client_name[CLIENTNAME+1];
    char billboards[NUMBER_ADS+1][ADD_SIZE+1];
    uint8_t number_ads;
    uint32_t price;
} client;

typedef struct {
    client client_list[CLIENTS+1];
    client* previous_client;
    uint32_t total_income;

} client_manager;


void next_mode(mode *m);
void next_effect(effect *e, client_manager* mgr);
void effect_output(effect e, client_manager* mgr, uint32_t *out);
void add_clients(client_manager *mgr);
client* next_client(client_manager *mgr);
void next_billboard(client_manager *mgr);

#endif