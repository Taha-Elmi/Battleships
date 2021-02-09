//
// Created by elmit on 2/8/2021.
//

#include "game_loops.h"
#include <stdio.h>

void fire(game* game1, map* map1, int column, char row) {
    int x = column - 1;
    int y = (int )row - 65;
    switch (map1->board[x][y].situation) {
        case Water:
        case Explotion:
        case Complete:
            printf("The block was already fired. Try again.\n");
            fire(game1, map1, column, row);
            return;
        case empty:
            map1->board[x][y].situation = Water;
            break;
        case full:
            (game1->turn == 1) ? (game1->current_score_1++) : (game1->current_score_2++);
            map1->board[x][y].situation = Explotion;
            break;
    }
}

int score_of_ships (int ship_size) {
    int max_score = 5 * ship_sizes[0];
    return (max_score / ship_size);
}

void check_ships(game *game1, ship** ship1, map* map1) {
    ship *iteration = (*ship1);
    while (iteration != NULL) {
        int non_hitted = iteration->size;
        switch (iteration->direction) {
            case horizental:
                for (int i = iteration->top_left.x; i <= iteration->bottom_right.x ; ++i) {
                    if (map1->board[i][iteration->top_left.y].situation == Explotion)
                        non_hitted--;
                }
                break;
            case vertical:
                for (int i = iteration->top_left.y; i <= iteration->bottom_right.y ; ++i) {
                    if (map1->board[iteration->top_left.x][i].situation == Explotion)
                        non_hitted--;
                }
                break;
        }


        //if the ship is completely destroyed
        if (non_hitted == 0) {
            for (int i = iteration->top_left.x - 1; i <= iteration->bottom_right.x + 1; ++i) {
                for (int j = iteration->top_left.y - 1; j <= iteration->bottom_right.y + 1; ++j) {
                    if ((i >= 0) && (i < map_size) && (j >= 0) && (j < map_size)) {
                        if (map1->board[i][j].situation == Explotion)
                            map1->board[i][j].situation = Complete;
                        else
                            map1->board[i][j].situation = Water;
                    }
                }
            }

            //give the score of destroying
            int destroyed_area = iteration->size;
            (game1->turn == 1) ? (game1->current_score_1 += score_of_ships(destroyed_area)) : (game1->current_score_2 += score_of_ships(destroyed_area));

            //remove the ship from linked list
            ship *delete = iteration;
            iteration = iteration->next;
            pop_ship(ship1, delete);
            continue;
        }

        iteration = iteration->next;
    }
}
