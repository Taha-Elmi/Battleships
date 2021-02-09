#include <stdio.h>
#include <stdlib.h>

#include "objects.h"
#include "ui.h"
#include "game_loops.h"


int main() {
    set_ship_sizes(&ship_sizes, 10, 2, 3, 3, 2, 5, 2, 1, 1, 1, 1);
    setup_player(&players);
    setup_player(&players);
    game *game1 = setup_game(&players[0], &players[1]);

    raw_draw(*(game1->player1.map));
    raw_draw(*(game1->player2.map));
    /*for (int i = 0; i < 5; ++i) {
        fire(map1, 1, (char )(i + 65));
        check_ships( &(players->ships) , map1);
        draw(*map1);
    }*/
    return 0;
}
