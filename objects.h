//
// Created by elmit on 2/5/2021.
//

#ifndef BATTLESHIPS_OBJECTS_H
#define BATTLESHIPS_OBJECTS_H

#endif //BATTLESHIPS_OBJECTS_H

extern int map_size;
extern int number_of_ships;
extern int *ship_sizes;

enum direction {horizental, vertical};
enum situation {empty, full, Water, Explotion, Complete};

typedef struct {
    int x;
    int y;
    enum situation situation; //it can be empty or full, which will not be visible to the opponent
                              //or it can be 'W', 'E', 'C', which will be visible to the opponent
} location;

struct ship {
    int size;
    int blocks_left;
    enum direction direction;
    location top_left;
    location bottom_right;
    struct ship* next;
};
typedef struct ship ship;

typedef struct {
    int size;
    location **board;
} map;

struct player {
    char name[20];
    int score;
    ship* ships;
    map* map;
};
typedef struct player player;
extern player *players;
extern int number_of_players;

typedef struct {
    player player1;
    int current_score_1;
    player player2;
    int current_score_2;
    int turn;
} game;


void set_ship_sizes(int **ship_sizes, int number_of_ships, ...);
void change_ship_sizes(int **ship_sizes, int number_of_ships);
void get_list(ship** list, int n, map* map1);
void pop_ship(ship** list, ship* ship1);
void creat_board(map* map1, int size);
void setup_player(player **list);
void sort_players(player* list);
int search_name(player* players, char name[]);
void setup_game();