#include <stdio.h>
#include <stdlib.h>
#include "objects.h"

int main() {
    set_ship_sizes(ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    setup_player(&players);
    map* map1 = (map *)malloc(sizeof(map));
    creat_board(map1, map_size);
    get_list(&players->ships, number_of_ships, map1);
    return 0;
}
