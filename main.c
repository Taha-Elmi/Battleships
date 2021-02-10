#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "objects.h"
#include "ui.h"
#include "game_loops.h"


int main() {
    set_ship_sizes(&ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    srand(time(NULL));
    setup_player(&players);
    map *map1 = (map*)malloc(sizeof(map));
    creat_board(map1, map_size);
    auto_get_list(&players->ships, map1);
    raw_draw(*map1);
    return 0;
}
