#include <stdio.h>
#include <stdlib.h>

#include "objects.h"
#include "ui.h"
#include "game_loops.h"


int main() {
    set_ship_sizes(&ship_sizes, 3, 6, 4, 3);
    setup_player(&players);
    setup_player(&players);
    game *game1 = setup_game(&players[0], &players[1]);

    for (int i = 0; i < 4; ++i) {
        multiplayer_round(game1);
    }
    return 0;
}
