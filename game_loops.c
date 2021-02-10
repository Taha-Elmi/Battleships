//
// Created by elmit on 2/8/2021.
//

#include "game_loops.h"
#include "ui.h"
#include <stdio.h>
#include <ctype.h>
#include <windows.h>

int valid_input(int column, char row) {
    row -= 64;
    if (column == -1 || column == -2)
        return column;
    if (column < 1 || column > map_size
        || row < 1 || row > map_size) {
        printf("Invalid input\n");
        return 0;
    }
}

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

int check_end(game* game1) {
    if (game1->player1->ships == NULL)
        return 2;
    if (game1->player2->ships == NULL)
        return 1;
    return 0;
}

void finish_game(game* game1, int winner) {
    system("cls");
    if (winner == 1) {
        printf("%s won the game :)\n", game1->player1->name);
        game1->player1->score += game1->current_score_1;
        game1->player2->score += (game1->current_score_2 / 2);
    } else {
        printf("%s won the game :)\n", game1->player1->name);
        game1->player2->score += game1->current_score_2;
        game1->player1->score += (game1->current_score_2 / 2);
    }
    free(game1->player1->ships);
    game1->player1->ships = NULL;
    free(game1->player2->ships);
    game1->player2->ships = NULL;
    free(game1);
}

void multiplayer_round (game* game1) {
    short column;
    char row;

    if (game1->turn == 1) {

        system("cls");
        draw(*game1->player2->map);
        printf("%s, your turn : ", game1->player1->name);
        scanf("%d%c", &column, &row);
        row = toupper(row);

        int input = valid_input(column, row);
        if (input == -1) {

            return;
        }
        if (input == -2) {

            return;
        }
        if (input == 0){
            printf("Invalid input. Choose a block or use jet power or save the game\n");
            multiplayer_round(game1);
            return;
        }

        fire(game1, game1->player2->map, column, row);
        check_ships(game1, &game1->player2->ships, game1->player2->map);
        system("cls");
        draw(*game1->player2->map);
        Sleep(2000);

        game1->turn = 2;
    } else {

        system("cls");
        draw(*game1->player1->map);
        printf("%s, your turn : ", game1->player2->name);
        scanf("%d%c", &column, &row);
        row = toupper(row);

        int input = valid_input(column, row);
        if (input == -1) {

            return;
        }
        if (input == -2) {

            return;
        }
        if (input == 0){
            printf("Invalid input. Choose a block or use jet power or save the game\n");
            multiplayer_round(game1);
            return;
        }

        fire(game1, game1->player1->map, column, row);
        check_ships(game1, &game1->player1->ships, game1->player1->map);
        system("cls");
        draw(*game1->player1->map);
        Sleep(2000);

        game1->turn = 1;
    }
}

void multiplayer(game* game1) {
    int end = 0;
    while (end == 0) {
        multiplayer_round(game1);
        end = check_end(game1);
    }
    finish_game(game1, end);
}