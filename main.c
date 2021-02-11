#include <stdlib.h>
#include <time.h>

#include "objects.h"
#include "ui.h"
#include "filing.h"


int main() {
    set_ship_sizes(&ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    load_players();
    srand(time(NULL));
    while (menu());
    return 0;
}
