//
// Created by elmit on 2/5/2021.
//

#include "objects.h"
#include "ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>

int map_size = 10;
int number_of_ships = 10;
int number_of_players = 0;
int *ship_sizes = NULL;
player* players = NULL;
player* bot = NULL;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int random(int lower, int upper) {
    int n = rand() % (upper - lower + 1) + lower;
    return n;
}

void sort_ship_sizes() {
    for (int i = number_of_ships; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (ship_sizes[j] < ship_sizes[j+1])
                swap(&ship_sizes[j], &ship_sizes[j+1]);
        }
    }
}

void set_ship_sizes(int **ship_sizes, int count, ...) {
    number_of_ships = count;
    (*ship_sizes) = (int *)calloc(count, sizeof(int ));
    if ((*ship_sizes) == NULL){
        printf("Crack!\n");
    }
    va_list sizes;
    va_start(sizes, count);
    for (int i = 0; i < count; ++i)
        (*ship_sizes)[i] = va_arg(sizes, int);
    va_end(sizes);
    sort_ship_sizes();
}

void change_ship_sizes(int **ship_sizes, int new_number_of_ships) {
    number_of_ships = new_number_of_ships;
    (*ship_sizes) = (int *)realloc((*ship_sizes), number_of_ships * sizeof(int ));
    int input;
    for (int i = 0; i < number_of_ships; ++i) {
        printf("Enter ship N.%d's size: ", i + 1);
        scanf("%d", &input);
        (*ship_sizes)[i] = input;
    }
    sort_ship_sizes();
}

void change_map_size(int new_size) {
    map_size = new_size;
}

void creat_board(map* map1, int size) {
    map1->size = size;
    map1->board = (location **)calloc(size, sizeof(location *));
    for (int i = 0; i < size; ++i) {
        map1->board[i] = (location *)calloc(size, sizeof(location ));
        for (int j = 0; j < size; ++j) {
            map1->board[i][j].x = i;
            map1->board[i][j].y = j;
            map1->board[i][j].situation = empty;
        }
    }
}

map* copy_board(map* map1) {
    map* map2 = (map*)malloc(sizeof(map));
    creat_board(map2, map1->size);
    for (int i = 0; i < map1->size; ++i) {
        for (int j = 0; j < map1->size; ++j)
            map2->board[i][j] = map1->board[i][j];
    }
    return map2;
}

void insert_ship(ship **list, ship new_ship){
    ship* p_new_ship = (ship*)malloc(sizeof(ship));
    (*p_new_ship) = new_ship;
    p_new_ship->next = NULL;
    if (*list == NULL) {
        *list = p_new_ship;
        return;
    }
    ship* iteration = *list;
    while (iteration->next != NULL)
        iteration = iteration->next;
    iteration->next = p_new_ship;
}

void swap_locations(location* a, location* b) {
    location temp = *a;
    *a = *b;
    *b = temp;
}

void get_ship_size_1(ship* ship1, map* map1) {
    ship1->size = 1;
    short column;
    char row;

    printf("Set the area of a ship with size 1\n");

    printf("Where's the ship's only coordinate (example: 1a) ? ");

    fflush(stdin);
    scanf("%d%c", &column, &row);
    row = toupper(row);
    if (isalpha(row) == 0) {
        printf("Invalid input :/ Try again\n");
        get_ship_size_1(ship1, map1);
        return;
    }
    ship1->top_left.x = column - 1;
    ship1->top_left.y = (int )row - 65;
    if (ship1->top_left.x < 0 || ship1->top_left.y < 0 || ship1->top_left.x >= map_size || ship1->top_left.y >= map_size) {
        printf("Invalid inputs :/\n");
        get_ship_size_1(ship1, map1);
        return;
    }
    ship1->bottom_right = ship1->top_left;

    //here we check if the area that the player chose is empty
    for (int i = ship1->top_left.x - 1; i <= ship1->bottom_right.x + 1; ++i) {
        for (int j = ship1->top_left.y - 1; j <= ship1->bottom_right.y + 1; ++j) {
            if ((i >= 0) && (i < map_size) && (j >= 0) && (j < map_size) && (map1->board[i][j].situation != empty)) {
                printf("The area you chose or the adjoining area was not completely empty.\n");
                get_ship_size_1(ship1, map1);
                return;
            }
        }
    }

    map1->board[ship1->top_left.x][ship1->top_left.y].situation = full;
    ship1->next = NULL;

}

void get_ship(ship* ship1, int size, map* map1) {
    if (size == 1) {
        get_ship_size_1(ship1, map1);
        return;
    }

    ship1->size = size;
    short column;
    char row;

    printf("Set the area of a ship with size %d\n", size);

    printf("Where's the ship's beginning coordinate (example: 1a) ? ");
    fflush(stdin);
    scanf("%d%c", &column, &row);
    row = toupper(row);
    if (isalpha(row) == 0) {
        printf("Invalid input :/ Try again\n");
        get_ship(ship1, size, map1);
        return;
    }
    ship1->top_left.x = column - 1;
    ship1->top_left.y = (int )row - 65;
    if (ship1->top_left.x < 0 || ship1->top_left.y < 0 || ship1->top_left.x >= map_size || ship1->top_left.y >= map_size) {
        printf("Invalid inputs :/\n");
        get_ship(ship1, size, map1);
        return;
    }

    printf("Where's the ship's tail-end coordinate (example: 1a) ? ");
    scanf("%d%c", &column, &row);
    row = toupper(row);
    if (isalpha(row) == 0) {
        printf("Invalid input :/ Try again\n");
        get_ship(ship1, size, map1);
        return;
    }
    ship1->bottom_right.x = column - 1;
    ship1->bottom_right.y = (int )row - 65;
    if (ship1->bottom_right.x < 0 || ship1->bottom_right.y < 0 || ship1->bottom_right.x >= map_size || ship1->bottom_right.y >= map_size) {
        printf("Invalid inputs :/\n");
        get_ship(ship1, size, map1);
        return;
    }


    //here we check if the points are in a straight way
    if (ship1->top_left.x != ship1->bottom_right.x && ship1->top_left.y != ship1->bottom_right.y) {
        printf("Wrong inputs, they must be in a straight way.\n");
        get_ship(ship1, size, map1);
        return;
    }

    //here we check to set the top_left point and the bottom_right point
    if (ship1->bottom_right.x < ship1->top_left.x || ship1->bottom_right.y < ship1->top_left.y)
        swap_locations(&ship1->top_left, &ship1->bottom_right);


    ship1->direction = (ship1->top_left.x == ship1->bottom_right.x) ? vertical : horizental;

    //here we check if the size of ship is correct
    switch (ship1->direction) {
        case vertical:
            if ( (ship1->bottom_right.y - ship1->top_left.y) != size-1) {
                printf("The size of area is not correct.\n");
                get_ship(ship1, size, map1);
                return;
            }
            break;
        case horizental:
            if ( (ship1->bottom_right.x - ship1->top_left.x) != size-1) {
                printf("The size of area is not correct.\n");
                get_ship(ship1, size, map1);
                return;
            }
            break;
    }

    //here we check if the area that the player chose is empty
    for (int i = ship1->top_left.x - 1; i <= ship1->bottom_right.x + 1; ++i) {
        for (int j = ship1->top_left.y - 1; j <= ship1->bottom_right.y + 1; ++j) {
            if ((i >= 0) && (i < map_size) && (j >= 0) && (j < map_size) && (map1->board[i][j].situation != empty)) {
                printf("The area you chose or the adjoining area was not completely empty.\n");
                get_ship(ship1, size, map1);
                return;
            }
        }
    }


    //here we update the map
    if (ship1->direction == horizental) {

        for (int i = ship1->top_left.x; i <= ship1->bottom_right.x ; ++i)
            map1->board[i][ship1->top_left.y].situation = full;

    } else {

        for (int i = ship1->top_left.y; i <= ship1->bottom_right.y ; ++i)
            map1->board[ship1->top_left.x][i].situation = full;

    }

}

void get_list(ship** list, map* map1, char* name) {
    system("cls");
    (*list) = NULL;
    for (int i = 0; i < number_of_ships; ++i) {
        ship temp;
        printf("%s's map:\n", name);
        raw_draw(*map1);
        get_ship(&temp, ship_sizes[i], map1);
        insert_ship(list, temp);
        system("cls");
    }
}

void auto_get_ship(ship* ship1, int size, map* map1) {
    ship1->size = size;

    int rand_result = random(0, 1);
    (rand_result == 0) ? (ship1->direction = vertical) : (ship1->direction = horizental);

    //if the size is larger than half of the map_size, then we put it on the border
    if (size > (map_size / 2)) {
        rand_result = random(0, 1); //to decide to put it on upper/left axis or on lower/right axis
        if (rand_result == 0) {

            rand_result = random(0, map_size - size);

            switch (ship1->direction) {
                case vertical:
                    ship1->top_left.x = 0;
                    ship1->top_left.y = rand_result;
                    ship1->bottom_right.x = 0;
                    ship1->bottom_right.y = (rand_result + size - 1);
                    break;
                case horizental:
                    ship1->top_left.x = rand_result;
                    ship1->top_left.y = 0;
                    ship1->bottom_right.x = (rand_result + size - 1);
                    ship1->bottom_right.y = 0;
                    break;
            }
        } else {

            rand_result = random(0, map_size - size);

            switch (ship1->direction) {
                case vertical:
                    ship1->top_left.x = map_size - 1;
                    ship1->top_left.y = rand_result;
                    ship1->bottom_right.x = map_size - 1;
                    ship1->bottom_right.y = (rand_result + size - 1);
                    break;
                case horizental:
                    ship1->top_left.x = rand_result;
                    ship1->top_left.y = map_size - 1;
                    ship1->bottom_right.x = (rand_result + size - 1);
                    ship1->bottom_right.y = map_size - 1;
                    break;
            }
        }
    } else { //size < (map_size / 2)

        switch (ship1->direction) {
            case vertical:

                rand_result = random(0, map_size - 1);
                ship1->top_left.x = rand_result;
                ship1->bottom_right.x = rand_result;

                rand_result = random(0, map_size - size);
                ship1->top_left.y = rand_result;
                ship1->bottom_right.y = rand_result + size - 1;

                break;

            case horizental:

                rand_result = random(0, map_size - 1);
                ship1->top_left.y = rand_result;
                ship1->bottom_right.y = rand_result;

                rand_result = random(0, map_size - size);
                ship1->top_left.x = rand_result;
                ship1->bottom_right.x = rand_result + size - 1;

                break;
        }
    }

    //here we check if the area is empty
    for (int i = ship1->top_left.x - 1; i <= ship1->bottom_right.x + 1; ++i) {
        for (int j = ship1->top_left.y - 1; j <= ship1->bottom_right.y + 1; ++j) {
            if ((i >= 0) && (i < map_size) && (j >= 0) && (j < map_size) && (map1->board[i][j].situation != empty)) {
                auto_get_ship(ship1, size, map1);
                return;
            }
        }
    }

    //here we update the map
    switch (ship1->direction) {
        case horizental:
            for (int i = ship1->top_left.x; i <= ship1->bottom_right.x ; ++i)
                map1->board[i][ship1->top_left.y].situation = full;
            break;

        case vertical:
            for (int i = ship1->top_left.y; i <= ship1->bottom_right.y ; ++i)
                map1->board[ship1->top_left.x][i].situation = full;
            break;
    }

}

void auto_get_list(ship** list, map* map1) {
    (*list) = NULL;
    for (int i = 0; i < number_of_ships; ++i) {
        ship temp;
        auto_get_ship(&temp, ship_sizes[i], map1);
        insert_ship(list, temp);
    }
}

void pop_ship(ship** list, ship* ship1) {
    ship *delete;
    if ((*list) == ship1) {
        delete = (*list);
        (*list) = (*list)->next;
        free(delete);
        return;
    }

    ship *iteration = (*list);
    while (iteration->next != ship1)
        iteration = iteration->next;
    delete = iteration->next;
    iteration->next = iteration->next->next;
    free(delete);
}

int search_name(char name[]) {
    for (int i = 0; i < number_of_players; ++i) {
        if (strcmp(players[i].name, name) == 0)
            return i;
    }
    return -1;
}

void setup_player(player **list) {
    if ((*list) == NULL) {
        (*list) = (player *)malloc(sizeof(player));
    } else {
        (*list) = (player *)realloc((*list), (number_of_players + 1) * sizeof(player));
    }

    fflush(stdin);
    printf("Enter your name: ");
    gets((*list)[number_of_players].name);

    //to avoid similar names, we check them and add number in case of similarity
    int index = 0;
    char temp_name[20];
    char temp_number[5];
    strcpy(temp_name, (*list)[number_of_players].name);
    while (search_name(temp_name) != -1) {
        index++;
        strcpy(temp_name, (*list)[number_of_players].name);
        sprintf(temp_number, "%d", index);
        strcat(temp_name, temp_number);
    }
    strcpy((*list)[number_of_players].name, temp_name);

    (*list)[number_of_players].score = 0;
    (*list)[number_of_players].ships = NULL;
    number_of_players++;
}

void swap_players(player* a, player* b) {
    player temp = *a;
    *a = *b;
    *b = temp;
}

void sort_players() {
    for (int i = (number_of_players - 1); i > 0 ; --i) {
        for (int j = 0; j < i; ++j) {
            if (players[j].score < players[j+1].score)
                swap_players(&players[j], &players[j+1]);
        }
    }
}

game* setup_multi_game(player* player1, player* player2) {
    game *game1 = (game*)malloc(sizeof(game));
    game1->game_mode = multi;

    map *map1 = (map *)malloc(sizeof(map));
    creat_board(map1, map_size);
    player1->map = map1;
    game1->current_score_1 = 0;
    game1->player1 = player1;
    if (player1->putting == manually)
        get_list(&player1->ships, player1->map, player1->name);
    else
        auto_get_list(&player1->ships, player1->map);

    map *map2 = (map *)malloc(sizeof(map));
    creat_board(map2, map_size);
    player2->map = map2;
    game1->current_score_2 = 0;
    game1->player2 = player2;
    if (player2->putting == manually)
        get_list(&player2->ships, player2->map, player2->name);
    else
        auto_get_list(&player2->ships, player2->map);


    //here we set rocket option
    system("cls");
    if (player1->score >= 100) {
        game1->rocket_1 = 1;
        printf("\n%s, you can use your rocket once in the game :)\n\n", player1->name);
    }
    else {
        game1->rocket_1 = 0;
        printf("\n%s, you cannot use any rocket because of lack of points :(\n\n", player1->name);
    }

    if (player2->score >= 100) {
        game1->rocket_2 = 1;
        printf("%s, you can use your rocket once in the game :)\n\n", player2->name);
    }
    else {
        game1->rocket_2 = 0;
        printf("%s, you cannot use any rocket because of lack of points :(\n\n", player2->name);
    }

    printf("Press any key to start the game ! ");
    getch();

    game1->is_saved = 0;
    game1->turn = 1;

    return game1;
}

game* setup_single_game(player* player1) {
    game *game1 = (game*)malloc(sizeof(game));
    game1->game_mode = single;

    map *map1 = (map *)malloc(sizeof(map));
    creat_board(map1, map_size);
    player1->map = map1;
    game1->current_score_1 = 0;
    game1->player1 = player1;
    if (player1->putting == manually)
        get_list(&player1->ships, player1->map, player1->name);
    else
        auto_get_list(&player1->ships, player1->map);

    bot = (player*)malloc(sizeof(player));
    strcpy(bot->name, "Bot");
    bot->ships = NULL;
    map *map2 = (map *)malloc(sizeof(map));
    creat_board(map2, map_size);
    bot->map = map2;
    game1->current_score_2 = 0;
    game1->player2 = bot;
    auto_get_list(&bot->ships, bot->map);

    //here we set rocket option
    system("cls");
    if (player1->score >= 100) {
        game1->rocket_1 = 1;
        printf("\n%s, you can use your rocket once in the game :)\n\n", player1->name);
    }
    else {
        game1->rocket_1 = 0;
        printf("\n%s, you cannot use any rocket because of lack of points :(\n\n", player1->name);
    }

    printf("Press any key to start the game !");
    getch();

    game1->is_saved = 0;
    game1->turn = 1;

    return game1;
}