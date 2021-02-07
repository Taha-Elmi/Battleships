//
// Created by elmit on 2/5/2021.
//

#ifndef BATTLESHIPS_SHIPS_H
#define BATTLESHIPS_SHIPS_H

#endif //BATTLESHIPS_SHIPS_H

extern int map_size;
extern int number_of_ships;
extern int *ship_sizes;

enum direction {horizental, vertical};
enum hit {fired, unfired};

typedef struct {
    int x;
    int y;
    char situation; //it can be '-' for emptyness, 'F' for being full(both will show '-' to the opponent
                    //or it can be 'W', 'E', 'C', which will be visible to the opponent
    enum hit hit;
} location;

struct ship {
    int size;
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
    struct player* next;
};
typedef struct player player;
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
int search_name(player* players, char name[]);
void setup_game();