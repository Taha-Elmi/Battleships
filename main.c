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
    game * game1 = setup_single_game(players);
    raw_draw(*game1->player1->map);
    printf("\nBot:\n\n");
    raw_draw(*game1->player2->map);
    return 0;
}
