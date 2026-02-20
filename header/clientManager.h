#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <stdint.h>

#define CLIENTNAME 50
#define NUMBER_ADS 3
#define ADD_LENGTH 16
#define CLIENTS 5



enum Mode {
    TEXT,
    ODD_EVEN
};

enum Effect {
    PLAIN_TEXT,
    SCROLL,
    BLINK
};

typedef struct Client {
    char* client_name[CLIENTNAME+1];
    char* billboards[NUMBER_ADS+1][ADD_LENGTH+1];
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
void next_effect(enum Effect *e);
void effect_output(enum Effect e, client_manager* mgr);


#endif