//
// Created by elmit on 2/8/2021.
//

#include "ui.h"
#include <stdio.h>

void draw(map map1) {
    printf("   ");
    for (int i = 0; i < map_size; ++i)
        printf(" %d ", i + 1);
    printf("\n");

    for (int i = 0; i < map_size; ++i) {
        printf(" %c ", i + 65);

        for (int j = 0; j < map_size; ++j) {
            switch (map1.board[i][j].situation) {
                case Water:
                    printf(" W ");
                    break;
                case Explotion:
                    printf(" E ");
                    break;
                case Complete:
                    printf(" C ");
                    break;
                default:
                    printf(" - ");
            }
        }

        printf("\n");
    }
}

void raw_draw(map map1) {
    printf("   ");
    for (int i = 0; i < map_size; ++i)
        printf(" %d ", i + 1);
    printf("\n");

    for (int i = 0; i < map_size; ++i) {
        printf(" %c ", i + 65);

        for (int j = 0; j < map_size; ++j) {
            switch (map1.board[i][j].situation) {
                case full:
                    printf(" F ");
                    break;
                default:
                    printf(" - ");
            }
        }

        printf("\n");
    }
}