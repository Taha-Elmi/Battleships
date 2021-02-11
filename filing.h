//
// Created by elmit on 2/11/2021.
//

#ifndef SHIPS_C_FILING_H
#define SHIPS_C_FILING_H

#endif //SHIPS_C_FILING_H

#ifndef BATTLESHIPS_OBJECTS_H
#include "objects.h"
#endif

typedef struct {
    char name[20];
    char player1[20];
    char player2[20];
    map map1;
    map map2;
    int ships_number_1;
    int ships_number_2;
    game game1;
    short game_status;
} game_to_save;

typedef struct {
    char game_name[20];
    ship ship1;
} ship_to_save;

typedef struct {
    char game_name[20];
    location location1;
} map_to_save;

void save_players();
void load_players();
void save_game(game game1);
void update_files();
int list_of_games ();
game* load_game(int number);
game* load_last_game();
