//
// Created by elmit on 2/5/2021.
//

#ifndef BATTLESHIPS_OBJECTS_H
#define BATTLESHIPS_OBJECTS_H

#endif //BATTLESHIPS_OBJECTS_H

//enums
enum direction {horizental, vertical};
enum situation {empty, full, Water, Explotion, Complete};
enum game_mode {single, multi};
enum putting {automatic, manually};


//structs
typedef struct {
    int x;
    int y;
    enum situation situation; //it can be empty or full, which will not be visible to the opponent
                              //or it can be 'W', 'E', 'C', which will be visible to the opponent
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

typedef struct {
    char name[20];
    int score;
    ship* ships;
    map* map;
    enum putting putting;
} player;

typedef struct {
    enum game_mode game_mode;

    player* player1;
    int current_score_1;
    int rocket_1;
    map* copy_map1;
    ship* copy_ships1;

    player* player2;
    int current_score_2;
    int rocket_2;
    map* copy_map2;
    ship* copy_ships2;

    int *columns;
    char *rows;
    int *copy_turn;
    int rounds;
    int is_saved;
    int turn;
} game;


//global variables
extern int map_size;
extern int number_of_ships;
extern int *ship_sizes;
extern int number_of_players;
extern player *players;
extern player *bot;


//functions
void set_ship_sizes(int **ship_sizes, int number_of_ships, ...);
void change_ship_sizes(int **ship_sizes, int number_of_ships);
void change_map_size(int new_size);
void get_list(ship** list, map* map1, char* name);
void auto_get_list(ship** list, map* map1);
void insert_ship(ship **list, ship new_ship);
void pop_ship(ship** list, ship* ship1);
void creat_board(map* map1, int size);
void setup_player(player **list);
void sort_players();
int search_name(char name[]);
game* setup_multi_game(player* player1, player* player2);
game* setup_single_game(player* player1);
int random(int lower, int upper);