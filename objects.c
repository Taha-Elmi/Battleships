//
// Created by elmit on 2/5/2021.
//

#include "objects.h"

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

void insert_player(player **list, player new_player){
    player* p_new_player = (player *)malloc(sizeof(player));
    (*p_new_player) = new_player;
    if (*list == NULL) {
        *list = p_new_player;
        return;
    }
    player* iteration = *list;
    while (iteration->next != NULL)
        iteration = iteration->next;
    iteration->next = p_new_player;
    return;
}

void setup_player(player **list) {
    player player1;
    fflush(stdin);
    printf("Enter your name: ");
    gets(player1.name);
    player1.score = 0;
    insert_player(list, player1);
    return;
}