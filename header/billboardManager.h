#ifndef BILLBOARDMANAGER_H
#define BILLBOARDMANAGER_H

#include "clientManager.h"
#include <stdint.h>

typedef enum  {
    SHOW_COMMERCIAL,
    SWITCH
} mode;

typedef struct {
    uint32_t minutes_ms;
    uint32_t text_ms;
    uint32_t scroll_ms;
    uint32_t blink_on_ms;
    uint32_t blink_off_ms;
    uint32_t client_ms;
} time;

typedef struct {
    mode mode;
    client* client;
    uint8_t billboard_index;
    time t;
    uint32_t min;
    uint8_t blink_on;
} player;

typedef enum selection_option soption;
typedef enum effects effect;

void player_state(client_manager *mgr, player *p, uint32_t now);
void player_init(player * play, client_manager *mgr, uint32_t now);
client* get_current_client(client_manager * mgr);

void set_minutes(client_manager *mgr, player *play, uint32_t now);
uint8_t get_billboard_index(player *play);
void print_billboard(client_manager *mgr, player *play, uint32_t now);
void print_client_name(player *play);

void run(client_manager *mgr, player *play, uint32_t now);

#endif