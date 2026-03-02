#include <stdlib.h>
#include "billboardManager.h"
#include "clientManager.h"
#include "lcd.h"
#include "utils.h"

static uint32_t minutes = 0;

void player_init(player *play, client_manager *mgr, uint32_t now){
    play->mode = SHOW_COMMERCIAL;
    play->client = get_current_client(mgr);
    play->billboard_index = get_billboard_index(play);
    play->t.client_ms = now;
    play->t.blink_ms = now;
    play->t.minutes_ms = now;
    play->t.scroll_ms = now;
    play->t.text_ms = now;
}

void player_state(client_manager *mgr, player *p, uint32_t now){

    if (p->mode == SHOW_COMMERCIAL){
        if ((uint32_t)(now - p->t.client_ms) >= 5000){
            p->mode = SWITCH;
            p->t.client_ms = now;
        }  
    }
     
    else if (p->mode == SWITCH){
        if ((uint32_t)(now - p->t.client_ms)>= mgr->intervals.switch_duration){
            lcd_clear();
            p->mode = SHOW_COMMERCIAL;
            next_client(mgr);
            p->client = get_current_client(mgr);
            p->billboard_index = get_billboard_index(p);
            p->t.client_ms = now;
            p->min = minutes;
            reset_offset();
        }
    }
    set_minutes(mgr, p, now);

}

client* get_current_client(client_manager *mgr){
    client *client = mgr->current_client;

    return client;
}

uint8_t  get_billboard_index(player *play){

    uint8_t available_billboards[NUMBER_ADS + 1];
    uint8_t available_count = 0;

    for (uint8_t i = 0; i <= NUMBER_ADS; i++){
        if (play->client->billboards[i].billboard[0] != '\0'){
            available_billboards[available_count++] = i;
        }
    }

    uint8_t billboard_index = 0;
    if (available_count > 0){
        billboard_index = available_billboards[rand() % available_count];
    }

    return billboard_index;
}


void set_minutes(client_manager *mgr, player *play, uint32_t now){
    if (now - play->t.minutes_ms >= mgr->intervals.minutes_tick){
        minutes++; 
        play->t.minutes_ms = now;
    }
}

void print_client_name(player *play){
    switch(play->client->client_id){
        case 1: {
            lcd_print(0, "Hederlige Harry.");
            break;
        }
        case 2: {
            lcd_print(0, "Farmor Anka.");
            break;
        }
        case 3: {
            lcd_print(0, "Svarte Petter.");
            break;
        }
        case 4: {
            lcd_print(0, "Janne Långben.");
            break;
        }
        case 5: {
            lcd_print(0, "IoT reklambyrå.");
            break;
        }
    }
}

void print_billboard(client_manager *mgr, player *play, uint32_t now){
    
    if (play->client->display_option == ODD_EVEN){
        if (play->min%2){
            print_client_name(play);
            if (now - play->t.scroll_ms >= mgr->intervals.scroll_tick){
                lcd_print_scroll(1, play->client->billboards[play->billboard_index].billboard);
                play->t.scroll_ms = now;
            }
        }else {
            print_client_name(play);
            if (now - play->t.text_ms >= mgr->intervals.scroll_tick){
                lcd_print_text(1, play->client->billboards[play->billboard_index].billboard);
                play->t.text_ms = now;
            }
        }
    }
    else {
        switch (play->client->billboards[play->billboard_index].effect){
            case TEXT: {
                print_client_name(play);
                if (now - play->t.text_ms >= mgr->intervals.scroll_tick){
                    lcd_print_text(1, play->client->billboards[play->billboard_index].billboard);
                    play->t.text_ms = now;
                }
                break;
            }
            case BLINK: {
                print_client_name(play);
                if(now - play->t.blink_ms >= mgr->intervals.blink_on){
                    lcd_print_blink(1, play->client->billboards[play->billboard_index].billboard);
                    play->t.blink_ms = now;
                }
                else if(now - play->t.blink_ms >= mgr->intervals.blink_off){
                    lcd_print_blink(1, play->client->billboards[play->billboard_index].billboard);
                    play->t.blink_ms = now;
                }
                break;
            }
            case SCROLL: {
                print_client_name(play);
                if (now - play->t.scroll_ms >= mgr->intervals.scroll_tick){
                    lcd_print_scroll(1, play->client->billboards[play->billboard_index].billboard);
                    play->t.scroll_ms = now;
                }
                break;
            }
        }
    }
}

void run(client_manager *mgr, player *play, uint32_t now){
    player_state(mgr, play, now);
    if (play->mode == SWITCH){
        return;
    }else{
        print_billboard(mgr, play, now);

    }
}

