#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "objects.h"
#include "ui.h"
#include "game_loops.h"


int main() {
    set_ship_sizes(&ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    srand(time(NULL));
    /*setup_player(&players);
    game * game1 = setup_single_game(players);
    single_player(game1);*/
    while (menu());
    return 0;
}
