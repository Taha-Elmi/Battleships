//
// Created by elmit on 2/8/2021.
//

#include "game_loops.h"
#include <stdio.h>

void fire(map* map1, int column, char row) {
    int x = column - 1;
    int y = (int )row - 65;
    switch (map1->board[x][y].situation) {
        case Water:
        case Explotion:
        case Complete:
            printf("The block was already fired. Try again.\n");
            fire(map1, column, row);
            return;
        case empty:
            map1->board[x][y].situation = Water;
            break;
        case full:
            map1->board[x][y].situation = Explotion;
            break;
    }
}

void multiplayer(game game1) {

}