//
// Created by elmit on 2/5/2021.
//

#ifndef BATTLESHIPS_SHIPS_H
#define BATTLESHIPS_SHIPS_H

#endif //BATTLESHIPS_SHIPS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern int map_size;
extern int number_of_ships;
extern int *ship_sizes;

enum direction {horizental, vertical};

typedef struct {
    int x;
    int y;
    char situation;
} location;

typedef struct {
    int size;
    enum direction direction;
    location top_left;
    location bottom_right;
    struct ship* next;
} ship;

typedef struct {
    int size;
    location **board;
} map;

typedef struct {
    char name[20];
    int score;
    ship* ships;
    map* map;
    struct player* next;
} player;
extern player *players;

typedef struct {
    player player1;
    player player2;
    map board;
} game;


void set_ship_sizes(int *ship_sizes, int number_of_ships, ...);
void get_list(ship** list, int n, map* map1);
void creat_board(map* map1, int size);
void setup_player(player **list);
void setup_game();