//
// Created by elmit on 2/11/2021.
//

#include "filing.h"
#include <stdio.h>
#include <stdlib.h>

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
    fread(&number_of_players, 1, sizeof(int ), players_file);
    players = (player*)calloc(number_of_players, sizeof(player));
    fread(players, number_of_players, sizeof(player), players_file);
    fclose(players_file);
}