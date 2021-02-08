//
// Created by elmit on 2/8/2021.
//

#ifndef SHIPS_C_GAME_LOOPS_H
#define SHIPS_C_GAME_LOOPS_H

#endif //SHIPS_C_GAME_LOOPS_H

#ifndef BATTLESHIPS_OBJECTS_H
#include "objects.h"
#endif

void single_player(game game1);
void multiplayer(game game1);
void fire(map* map1, int column, char row);
void check_ships(ship** ship1, map* map1); //to convert 'E' situation to 'C' situation and update scores
int check_finish(game game1);