#include <stdio.h>
#include "objects.h"

int main() {
    set_ship_sizes(ship_sizes, 10, 5, 3, 3, 2, 2, 2, 1, 1, 1, 1);
    int choice;
    scanf("%d", &choice);
    if (choice)
        setup_player(&players);
    printf("%s", players->name);
    return 0;
}
