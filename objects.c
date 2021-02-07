//
// Created by elmit on 2/5/2021.
//

#include "objects.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int map_size = 10;
int number_of_ships = 10;
int *ship_sizes;
player* players = NULL;

void set_ship_sizes(int *ship_sizes, int number_of_ships, ...) {
    ship_sizes = (int *)calloc(number_of_ships, sizeof(int ));
    va_list sizes;
    va_start(sizes, number_of_ships);
    for (int i = 0; i < number_of_ships; ++i)
        ship_sizes[i] = va_arg(sizes, int);
    va_end(sizes);
}

void creat_board(map* map1, int size) {
    map1->size = size;
    map1->board = (location **)calloc(size, sizeof(location *));
    for (int i = 0; i < size; ++i) {
        map1->board[i] = (location *)calloc(size, sizeof(location ));
        for (int j = 0; j < size; ++j) {
            map1->board[i][j].situation = '-';
        }
    }
}

void insert_ship(ship **list, ship new_ship){
    ship* p_new_ship = (ship*)malloc(sizeof(ship));
    (*p_new_ship) = new_ship;
    if (*list == NULL) {
        *list = p_new_ship;
        return;
    }
    ship* iteration = *list;
    while (iteration->next != NULL)
        iteration = iteration->next;
    iteration->next = p_new_ship;
    return;
}

void swap_locations (location* a, location* b) {
    location temp = *a;
    *a = *b;
    *b = temp;
}

void get_ship(ship* ship1, int size, map* map1) {
    ship1->size = size;
    printf("Where's the ship's beginning coordinate? ");
    scanf("%d%d", &ship1->top_left.x, &ship1->top_left.y);
    printf("Where's the ship's tail-end coordinate? ");
    scanf("%d%d", &ship1->bottom_right.x, &ship1->bottom_right.y);

    if (ship1->top_left.x != ship1->bottom_right.x && ship1->top_left.y != ship1->bottom_right.y) {
        printf("Wrong inputs :/\n");
        get_ship(ship1, size, map1);
    }

    //check other errors

    if (ship1->bottom_right.x > ship1->top_left.x || ship1->bottom_right.y > ship1->top_left.y)
        swap_locations(&ship1->top_left, &ship1->bottom_right);


    ship1->direction = (ship1->top_left.x == ship1->bottom_right.x) ? vertical : horizental;
    ship1->next = NULL;
    return;
}

void get_list(ship** list, int n, map* map1) {
    for (int i = 0; i < n; ++i) {
        ship temp;
        get_ship(&temp, ship_sizes[i], map1);
        insert_ship(list, temp);
    }
    return;
}

void insert_player(player **list, player* new_player){
    if (*list == NULL) {
        *list = new_player;
        return;
    }
    player* iteration = *list;
    while (iteration->next != NULL)
        iteration = iteration->next;
    iteration->next = new_player;
    return;
}

int search_name(player* players, char name[]) {
    int index = 0;
    while (players != NULL) {
        if (strcmpi(players->name, name) == 0)
            return index;
        players = players->next;
        index++;
    }
    return -1;
}

void setup_player(player **list) {
    player* player1 = (player*)malloc(sizeof(player ));
    fflush(stdin);
    printf("Enter your name: ");
    gets(player1->name);

    //to avoid similar names, we check them and add number in case of similarity
    int index = 0;
    char temp_name[20];
    char temp_number[5];
    strcpy(temp_name, player1->name);
    while (search_name(players, temp_name) != -1) {
        index++;
        strcpy(temp_name, player1->name);
        sprintf(temp_number, "%d", index);
        strcat(temp_name, temp_number);
    }
    strcpy(player1->name, temp_name);

    player1->score = 0;
    player1->next = NULL;
    insert_player(list, player1);
    return;
}

void setup_game(game* game1, player* player1, player* player2) {
    map *map1 = (map *)malloc(sizeof(map));
    creat_board(map1, map_size);
    player1->map = map1;
    get_list(&player1->ships, number_of_ships, player1->map);

    map *map2 = (map *)malloc(sizeof(map));
    creat_board(map2, map_size);
    player2->map = map2;
    get_list(&player2->ships, number_of_ships, player2->map);
}