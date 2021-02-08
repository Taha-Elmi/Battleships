#include <stdio.h>
#include <stdlib.h>

#include "objects.h"
#include "ui.h"
#include "game_loops.h"


int main() {
    set_ship_sizes(&ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", ship_sizes[i]);
    }
    setup_player(&players);
    map* map1 = (map *)malloc(sizeof(map));
    creat_board(map1, map_size);
    get_list(&(players->ships), number_of_ships, map1);
    raw_draw(*map1);
    fire(map1, 1, 'A');
    fire(map1, 2, 'A');
    draw(*map1);
    return 0;
}
