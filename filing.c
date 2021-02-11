//
// Created by elmit on 2/11/2021.
//

#include "filing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void save_players(){
    FILE *players_file;
    players_file = fopen("players.bin", "wb");
    fwrite(&number_of_players, 1, sizeof(int ), players_file);
    fwrite(players, number_of_players, sizeof(player), players_file);
    fclose(players_file);
}

void load_players() {
    FILE *players_file;
    players_file = fopen("players.bin", "rb");

    if (players_file == NULL)
        return;

    fread(&number_of_players, 1, sizeof(int ), players_file);
    players = (player*)calloc(number_of_players, sizeof(player));
    fread(players, number_of_players, sizeof(player), players_file);

    for (int i = 0; i < number_of_players; ++i)
        players[i].ships = NULL;

    fclose(players_file);
}

void save_game(game game1) {
    game_to_save tmp;
    printf("Enter a name to save the game:\n");
    fflush(stdin);
    gets(tmp.name);

    tmp.game1 = game1;
    strcpy(tmp.player1, game1.player1->name);
    strcpy(tmp.player2, game1.player2->name);
    tmp.map1 = *game1.player1->map;
    tmp.map2 = *game1.player2->map;

    int n = 0;
    ship* iteration = game1.player1->ships;
    while (iteration != NULL) {
        n++;
        iteration = iteration->next;
    }
    tmp.ships_number_1 = n;

    n = 0;
    iteration = game1.player2->ships;
    while (iteration != NULL) {
        n++;
        iteration = iteration->next;
    }
    tmp.ships_number_2 = n;

    tmp.game_status = 1; //it means it's not finished yet

    FILE *games_file;
    games_file = fopen("games.bin", "ab");
    fwrite(&tmp, 1, sizeof(game_to_save), games_file);
    fclose(games_file);

    FILE *ships_file;
    ships_file = fopen("ships.bin", "ab");
    for (int i = 0; i < tmp.ships_number_1; ++i) {
        ship_to_save to_save;
        strcpy(to_save.game_name, tmp.name);
        to_save.ship1 = *game1.player1->ships;
        fwrite(&to_save, 1, sizeof(ship_to_save), ships_file);
        game1.player1->ships = game1.player1->ships->next;
    }
    for (int i = 0; i < tmp.ships_number_2; ++i) {
        ship_to_save to_save;
        strcpy(to_save.game_name, tmp.name);
        to_save.ship1 = *game1.player2->ships;
        fwrite(&to_save, 1, sizeof(ship_to_save), ships_file);
        game1.player2->ships = game1.player2->ships->next;
    }
    fclose(ships_file);
}

void update_files() {
    FILE *games_file;
    games_file = fopen("games.bin", "rb");
    FILE *ships_file;
    ships_file = fopen("ships.bin", "rb");

    FILE *temp_games_file;
    temp_games_file = fopen("tmpgames.bin", "wb");
    FILE *temp_ships_file;
    temp_ships_file = fopen("tmpships.bin", "wb");

    while (1) {
        game_to_save temp_game;
        fread(&temp_game, 1, sizeof(game_to_save), games_file);

        if (feof(games_file))
            break;

        if (temp_game.game_status) {
            fwrite(&temp_game, 1, sizeof(game_to_save), temp_games_file);
            for (int i = 0; i < temp_game.ships_number_1; ++i) {
                ship_to_save temp_ship;
                fread(&temp_ship, 1, sizeof(ship_to_save), ships_file);
                fwrite(&temp_ship, 1, sizeof(ship_to_save), temp_ships_file);
            }
            for (int i = 0; i < temp_game.ships_number_2; ++i) {
                ship_to_save temp_ship;
                fread(&temp_ship, 1, sizeof(ship_to_save), temp_ships_file);
                fwrite(&temp_ship, 1, sizeof(ship_to_save), temp_ships_file);
            }
        } else {
            fseek(ships_file, (temp_game.ships_number_1 + temp_game.ships_number_2) * sizeof(ship_to_save), SEEK_CUR);
        }

    }

    fclose(games_file);
    fclose(ships_file);
    fclose(temp_games_file);
    fclose(temp_ships_file);

    games_file = fopen("games.bin", "wb");
    ships_file = fopen("ships.bin", "wb");
    temp_games_file = fopen("tmpgames.bin", "rb");
    temp_ships_file = fopen("tmpships.bin", "rb");

    while (1) {
        game_to_save temp;
        fread(&temp, 1, sizeof(game_to_save), temp_games_file);
        if (feof(temp_games_file))
            break;
        fwrite(&temp, 1, sizeof(game_to_save), games_file);
    }
    while (1) {
        ship_to_save temp;
        fread(&temp, 1, sizeof(ship_to_save ), temp_ships_file);
        if (feof(temp_ships_file))
            break;
        fwrite(&temp, 1, sizeof(ship_to_save ), ships_file);
    }

    fclose(games_file);
    fclose(ships_file);
    fclose(temp_games_file);
    fclose(temp_ships_file);
}

int list_of_games () {
    FILE *games_file;
    games_file = fopen("games.bin", "rb");
    int i = 0;
    while (1) {
        game_to_save temp;
        fread(&temp, 1, sizeof(game_to_save), games_file);
        if (feof(games_file))
            break;
        i++;
        printf("%d. %s\n", i, temp.name);
    }
    fclose(games_file);
    return i;
}

game* load_game(int number) {
    FILE *games_file;
    games_file = fopen("games.bin", "rb");
    FILE *ships_file;
    ships_file = fopen("ships.bin", "rb");

    fseek(games_file, (number - 1) * sizeof(game_to_save), SEEK_SET);
    game_to_save load;
    fread(&load, 1, sizeof(game_to_save), games_file);

    game *game1 = (game*)malloc(sizeof(game));
    *game1 = load.game1;
    game1->player1 = &players[search_name(load.player1)];
    game1->player2 = &players[search_name(load.player2)];
    map *map1 = (map*)malloc(sizeof(map));
    *map1 = load.map1;
    map *map2 = (map*)malloc(sizeof(map));
    *map2 = load.map2;
    game1->player1->map = map1;
    game1->player2->map = map2;
    game1->player1->ships = NULL;
    game1->player2->ships = NULL;
    game1->is_saved = 1;

    ship_to_save temp;
    fread(&temp, 1, sizeof(ship_to_save), ships_file);
    while (strcmp(temp.game_name , load.name))
        fread(&temp, 1, sizeof(ship_to_save), ships_file);
    for (int i = 0; i < load.ships_number_1; ++i) {
        insert_ship(&game1->player1->ships, temp.ship1);
        fread(&temp, 1, sizeof(ship_to_save), ships_file);
    }
    for (int i = 0; i < (load.ships_number_2 - 1); ++i) {
        insert_ship(&game1->player2->ships, temp.ship1);
        fread(&temp, 1, sizeof(ship_to_save), ships_file);
    }
    insert_ship(&game1->player2->ships, temp.ship1);
    load.game_status = 0;
    return game1;
}

game* load_last_game() {
    FILE *games_file;
    games_file = fopen("games.bin", "rb");
    FILE *ships_file;
    ships_file = fopen("ships.bin", "rb");

    fseek(games_file, -1 * sizeof(game_to_save), SEEK_END);
    game_to_save load;
    fread(&load, 1, sizeof(game_to_save), games_file);

    game *game1 = (game*)malloc(sizeof(game));
    *game1 = load.game1;
    game1->player1 = &players[search_name(load.player1)];
    game1->player2 = &players[search_name(load.player2)];
    map *map1 = (map*)malloc(sizeof(map));
    *map1 = load.map1;
    map *map2 = (map*)malloc(sizeof(map));
    *map2 = load.map2;
    game1->player1->map = map1;
    game1->player2->map = map2;
    game1->player1->ships = NULL;
    game1->player2->ships = NULL;
    game1->is_saved = 1;

    fseek(ships_file, -1 * (load.ships_number_1 + load.ships_number_2) * sizeof(ship_to_save), SEEK_END);
    ship_to_save temp;
    for (int i = 0; i < load.ships_number_1; ++i) {
        fread(&temp, 1, sizeof(ship_to_save), ships_file);
        insert_ship(&game1->player1->ships, temp.ship1);
    }
    for (int i = 0; i < load.ships_number_2; ++i) {
        fread(&temp, 1, sizeof(ship_to_save), ships_file);
        insert_ship(&game1->player2->ships, temp.ship1);
    }
    load.game_status = 0;
    return game1;
}