//
// Created by elmit on 2/8/2021.
//

#include "game_loops.h"
#include "ui.h"
#include "filing.h"

#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>

int valid_input(int column, char row, map map1) {
    row -= 64;
    if (column == -1 || column == -2)
        return column;
    if (column > 0 && column <= map_size
        && row > 0 && row <= map_size
        && (map1.board[column - 1][row - 1].situation == empty
        || map1.board[column - 1][row - 1].situation == full))
        return 1;
    return 0;
}

void fire(game* game1, map* map1, int column, char row) {

    int x = column - 1;
    int y = (int )row - 65;

    switch (map1->board[x][y].situation) {
        case Water:
        case Explotion:
        case Complete:
            printf("The block was already fired. Try again.\n");
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

void rocket(game* game1) {
    int column;
    char row;
    player *current_player = (game1->turn == 1) ? game1->player1 : game1->player2;
    player *opponent_player = (game1->turn == 1) ? game1->player2 : game1->player1;

    map* map1 = (game1->turn == 1) ? game1->player2->map : game1->player1->map;
    
    printf("Which direction?\n1. vertical\n2. horizontal\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            printf("Which column?\n");
            scanf("%d", &column);
            for (row = 'A'; row < (char )('A' + game1->player1->map->size); ++row) {
                fire(game1, map1, column, row);
                check_ships(game1, &opponent_player->ships, map1);
                system("cls");
                display_scores(game1);
                draw(*map1);
                Sleep(1000);
                if (map1->board[column - 1][(int )row - 65].situation != Water)
                    break;
            }
            break;

        case 2:
            printf("Which row?\n");
            fflush(stdin);
            scanf("%c", &row);
            row = toupper(row);
            for (column = 1; column < (1 + map1->size); ++column) {
                fire(game1, map1, column, row);
                check_ships(game1, &opponent_player->ships, map1);
                system("cls");
                display_scores(game1);
                draw(*map1);
                Sleep(1000);
                if (map1->board[column - 1][(int )row - 65].situation != Water)
                    break;
            }
            break;

        default:
            printf("Invalid input. Try again.\n");
            rocket(game1);
            return;
    }

    current_player->score -= 100;
    (game1->turn == 1) ? (game1->rocket_1 --) : (game1->rocket_2 --);
    (game1->turn == 1) ? (game1->turn = 2) : (game1->turn = 1);
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

void display_scores(game* game1) {
    printf("\t%s: %d\t%s: %d\n", game1->player1->name, game1->current_score_1, game1->player2->name, game1->current_score_2);
}

int check_end(game* game1) {
    if (game1->player1->ships == NULL)
        return 2;
    if (game1->player2->ships == NULL)
        return 1;
    if (game1->is_saved == 1)
        return -1;
    return 0;
}

void finish_game(game* game1, int winner) {
    system("cls");
    switch (winner) {
        case 1:
            printf("%s won the game :)\n", game1->player1->name);
            game1->player1->score += game1->current_score_1;
            game1->player2->score += (game1->current_score_2 / 2);
            break;
        case 2:
            printf("%s won the game :)\n", game1->player2->name);
            game1->player2->score += game1->current_score_2;
            game1->player1->score += (game1->current_score_2 / 2);
            break;
    }

    free(game1->player1->map);
    free(game1->player1->ships);
    game1->player1->ships = NULL;
    game1->player1->map = NULL;
    free(game1->player2->map);
    free(game1->player2->ships);
    game1->player2->map = NULL;
    game1->player2->ships = NULL;

    if (game1->game_mode == single)
        free(game1->player2);

    free(game1);

    printf("\nPress any key to exit");
    getch();
}

void multiplayer_round (game* game1) {
    short column;
    char row;

    if (game1->turn == 1) {

        system("cls");
        display_scores(game1);
        draw(*game1->player2->map);
        printf("%s, your turn : ", game1->player1->name);

        scanf("%d", &column);

        if (column == -1) {
            save_game(*game1);
            return;
        }

        if (column == -2) {
            if (game1->rocket_1 == 0) {
                printf("Sorry :( You don't have any rocket to use.\n");
                Sleep(1500);
                multiplayer_round(game1);
                return;
            }
            rocket(game1);
            return;
        }

        scanf("%c", &row);
        row = toupper(row);
        int input = valid_input(column, row, *game1->player2->map);

        if (input == 0){
            printf("Invalid input. Choose a block or use jet power or save the game\n");
            Sleep(1500);
            multiplayer_round(game1);
            return;
        }

        fire(game1, game1->player2->map, column, row);
        check_ships(game1, &game1->player2->ships, game1->player2->map);
        system("cls");
        display_scores(game1);
        draw(*game1->player2->map);

        game1->turn = 2;
    } else {

        system("cls");
        display_scores(game1);
        draw(*game1->player1->map);
        printf("%s, your turn : ", game1->player2->name);

        scanf("%d%c", &column);

        if (column == -1) {
            save_game(*game1);
            return;
        }

        if (column == -2) {
            if (game1->rocket_2 == 0) {
                printf("Sorry :( You don't have any rocket to use.\n");
                Sleep(1500);
                multiplayer_round(game1);
                return;
            }
            rocket(game1);
            return;
        }

        scanf("%c", &row);
        row = toupper(row);
        int input = valid_input(column, row, *game1->player1->map);

        if (input == 0){
            printf("Invalid input. Choose a block or use jet power or save the game\n");
            Sleep(1500);
            multiplayer_round(game1);
            return;
        }

        fire(game1, game1->player1->map, column, row);
        check_ships(game1, &game1->player1->ships, game1->player1->map);
        system("cls");
        display_scores(game1);
        draw(*game1->player1->map);

        game1->turn = 1;
    }
    Sleep(1500);
}

void multiplayer(game* game1) {
    int end = 0;
    while (end == 0) {
        multiplayer_round(game1);
        end = check_end(game1);
    }
    finish_game(game1, end);
}

int is_E(map map1, location* E) {
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            if (map1.board[i][j].situation == Explotion) {
                *E = map1.board[i][j];
                return 1;
            }
        }
    }
    return 0;
}

location clever_pickup(map map1, location guess) {
    //define the direction
    enum direction guess_direction;
    if ((guess.y - 1 >= 0 && map1.board[guess.x][guess.y - 1].situation == Explotion)
        || (guess.y + 1 < map_size && map1.board[guess.x][guess.y + 1].situation == Explotion))
        guess_direction = vertical;
    else if ((guess.x - 1 >= 0 && map1.board[guess.x - 1][guess.y].situation != Water)
             || (guess.x + 1 < map_size && map1.board[guess.x + 1][guess.y].situation != Water))
        guess_direction = horizental;
    else
        guess_direction = vertical;

    //decide a block
    switch (guess_direction) {
        case horizental:
            while (guess.x + 1 < map_size
                && map1.board[guess.x + 1][guess.y].situation == Explotion)
                guess.x ++;
            if (guess.x + 1 < map_size && map1.board[guess.x + 1][guess.y].situation != Water) {
                guess.x ++;
                return guess;
            }
            while (map1.board[--guess.x][guess.y].situation == Explotion);
            return guess;
        case vertical:
            while (guess.y + 1 < map_size
                   && map1.board[guess.x][guess.y + 1].situation == Explotion)
                guess.y ++;
            if (guess.y + 1 < map_size && map1.board[guess.x][guess.y + 1].situation != Water) {
                guess.y ++;
                return guess;
            }
            while (map1.board[guess.x][--guess.y].situation == Explotion);
            return guess;
    }
}

void single_player_round(game* game1) {
    short column;
    char row;

    if (game1->turn == 1) {

        system("cls");
        display_scores(game1);
        draw(*game1->player2->map);
        printf("%s, your turn : ", game1->player1->name);

        scanf("%d", &column);

        if (column == -1) {
            save_game(*game1);
            return;
        }
        if (column == -2) {
            if (game1->rocket_1 == 0) {
                printf("Sorry :( You don't have any rocket to use.\n");
                Sleep(1500);
                multiplayer_round(game1);
                return;
            }
            rocket(game1);
            return;
        }

        scanf("%c", &row);
        row = toupper(row);
        int input = valid_input(column, row, *game1->player2->map);
        if (input == 0){
            printf("Invalid input. Choose a block or use jet power or save the game\n");
            Sleep(1500);
            multiplayer_round(game1);
            return;
        }

        fire(game1, game1->player2->map, column, row);
        check_ships(game1, &game1->player2->ships, game1->player2->map);
        system("cls");
        display_scores(game1);
        draw(*game1->player2->map);

        game1->turn = 2;
    } else {
        location guess;
        if (is_E(*game1->player1->map, &guess)) {
            guess = clever_pickup(*game1->player1->map, guess);
        } else {
            do {
                guess.x = random(0, map_size - 1);
                guess.y = random(0, map_size - 1);
            } while (game1->player1->map->board[guess.x][guess.y].situation != empty
                    && game1->player1->map->board[guess.x][guess.y].situation != full);
        }
        fire(game1, game1->player1->map, guess.x + 1, (char )(guess.y + 65));
        check_ships(game1, &game1->player1->ships, game1->player1->map);
        system("cls");
        display_scores(game1);
        draw(*game1->player1->map);

        game1->turn = 1;
    }
    Sleep(1500);
}

void single_player(game* game1) {
    int end = 0;
    while (end == 0) {
        single_player_round(game1);
        end = check_end(game1);
    }
    finish_game(game1, end);
}