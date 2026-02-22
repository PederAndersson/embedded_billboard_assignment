#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "lcd.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#define CLIENTNAME 50
#define ADD_SIZE 31
#define NUMBER_ADS 2
#define PARSE_BUFFER 254

#define CLIENTS 5
#define SCROLL_MS 200
#define BLINK_MS 350




enum Mode {
    TEXT,
    ODD_EVEN
};

enum Effect {
    PLAIN_TEXT,
    SCROLL,
    BLINK
};

typedef enum {
    S_CLIENT,
    S_BILLBOARD,
    S_PRICE,
    S_DONE
}parse_state;

typedef struct Client {
    char client_name[CLIENTNAME+1];
    char billboards[NUMBER_ADS+1][ADD_SIZE+1];
    uint32_t price;
} client;

typedef struct Client_manager {
    client* client_list[CLIENTS+1];
    client previous_client;
    uint32_t total_income;

} client_manager;


void manager_config(client *c, client_manager* mgr);
void next_client();
void next_mode(enum Mode *m);
void next_effect(enum Effect *e, client_manager* mgr);
void effect_output(enum Effect e, client_manager* mgr, uint32_t *out);
client parse_client_info(uint8_t row);
client_manager add_clients();


#endif