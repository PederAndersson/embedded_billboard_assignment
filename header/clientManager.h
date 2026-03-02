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

#define MINUTE_MS 60000
#define BILLBOARDS_MS 20000
#define SCROLL_MS 300
#define BLINK_ON 1500
#define BLINK_OFF 500
#define TEXT_SWITCH 2000
#define SWITCH_MS 2500

enum selection_option{
    RANDOM,
    ODD_EVEN
};

enum effects{
    TEXT,
    SCROLL,
    BLINK
};

typedef struct {
    char billboard[ADD_SIZE+1];
    uint8_t effect;
}billboard;


typedef enum {
    S_CLIENT,
    S_BILLBOARD,
    S_PRICE,
    S_ADS,
    S_ID,
    S_EFFECT,
    S_SELECTION,
    S_DONE
}parse_state;


typedef struct Interval {
  uint32_t billboard_duration;
  uint32_t scroll_tick;
  uint32_t blink_on;
  uint32_t blink_off;
  uint32_t text_switch;
  uint32_t minutes_tick;
  uint32_t switch_duration;
} default_interval;

typedef struct {
    char client_name[CLIENTNAME+1];
    billboard billboards[NUMBER_ADS+1];
    uint8_t display_option;
    uint8_t number_ads;
    uint8_t client_id;
    uint32_t price;
} client;

typedef struct {
    client client_list[CLIENTS+1];
    client* previous_client;
    client* current_client;
    uint32_t total_income;
    default_interval intervals;
} client_manager;

void add_clients(client_manager *mgr);
void next_client(client_manager *mgr);

#endif