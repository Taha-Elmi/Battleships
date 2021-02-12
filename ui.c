//
// Created by elmit on 2/8/2021.
//

#include "ui.h"
#include "game_loops.h"
#include "filing.h"

#include <stdio.h>
#include <conio.h>
#include <windows.h>

void draw(map map1) {
    printf("   ");
    for (int i = 0; i < map1.size; ++i)
        printf(" %d ", i + 1);
    printf("\n");

    for (int i = 0; i < map1.size; ++i) {
        printf(" %c ", i + 65);

        for (int j = 0; j < map1.size; ++j) {
            switch (map1.board[j][i].situation) {
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
            switch (map1.board[j][i].situation) {
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

int choose_player() {
    if (number_of_players == 0)
        return -1;

    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    for (int i = 0; i < number_of_players; ++i)
        printf("%d. %s\n", i+1, players[i].name);
    int choice;
    scanf("%d", &choice);
    if (choice <= number_of_players)
        return choice - 1;
    printf("Invalid input. Just enter the number of the player you want to choose.\n");
    Sleep(1500);
    return choose_player();
}

void menu_1() {
    int index_player1;
    enum putting how_player1;
    int index_player2;
    enum putting how_player2;

    //first player - choose user
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("First player :\n"
           "\nchoose user\n"
           "1. choose from available users\n"
           "2. new user\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            index_player1 = choose_player();
            if (index_player1 == -1) {
                printf("There is no user to choose yet. Creat one.\n");
                Sleep(2000);
                menu_1();
                return;
            }
            break;
        case 2:
            setup_player(&players);
            index_player1 = number_of_players - 1;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //first player - put ships
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("First player :\n"
           "\nput ships\n"
           "1. Auto\n"
           "2. Manual\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            how_player1 = automatic;
            break;
        case 2:
            how_player1 = manually;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //second player - choose user
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("Second player :\n"
           "1. choose from available users\n"
           "2. new user\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            index_player2 = choose_player();
            if (index_player2 == -1) {
                printf("There is no user to choose yet. Creat one.\n");
                Sleep(2000);
                menu_1();
                return;
            }
            break;
        case 2:
            setup_player(&players);
            index_player2 = number_of_players - 1;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //second player - put ships
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("Second player :\n"
           "\nput ships\n"
           "1. Auto\n"
           "2. Manual\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            how_player2 = automatic;
            break;
        case 2:
            how_player2 = manually;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //get started
    players[index_player1].putting = how_player1;
    players[index_player2].putting = how_player2;
    game *game1 = setup_multi_game(&players[index_player1], &players[index_player2]);
    multiplayer(game1);
}

void menu_2() {
    int index_player;
    enum putting how_player;

    //choose user
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("choose user\n"
           "1. choose from available users\n"
           "2. new user\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            index_player = choose_player();
            if (index_player == -1) {
                printf("There is no user to choose yet. Creat one.\n");
                Sleep(2000);
                menu_2();
                return;
            }
            break;
        case 2:
            setup_player(&players);
            index_player = number_of_players - 1;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //put ships
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("put ships\n"
           "1. Auto\n"
           "2. Manual\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            how_player = automatic;
            break;
        case 2:
            how_player = manually;
            break;
        default:
            printf("Wrong input. Choose a valid option.\n");
            menu_1();
            return;
    }

    //get started
    players[index_player].putting = how_player;
    game *game1 = setup_single_game(&players[index_player]);
    single_player(game1);
}

void menu_3() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    int n = list_of_games();

    if (n == 0) {
        printf("No game to load.");
        Sleep(1500);
        return;
    }

    int choice;
    printf("Choose one file with the appropriate number: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > n){
        printf("Invalid input. Try again.\n");
        Sleep(1500);
        menu_3();
        return;
    }

    game* game1 = load_game(n);
    switch (game1->game_mode) {
        case single:
            single_player(game1);
            update_files();
            return;
        case multi:
            multiplayer(game1);
            update_files();
            return;
    }
}

void menu_4() {
    int n = list_of_games();
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    if (n == 0) {
        printf("No game to load.\n");
        Sleep(1500);
        return;
    }
    game *game1 = load_last_game();
    switch (game1->game_mode) {
        case single:
            single_player(game1);
            update_files();
            return;
        case multi:
            multiplayer(game1);
            update_files();
            return;
    }
}

void menu_5_1() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");

    int new_number_of_ships;
    printf("How many ships do you want?\n");
    scanf("%d", &new_number_of_ships);
    if (new_number_of_ships < 1 || new_number_of_ships > (2 * map_size)) {
        printf("Please enter a logical number :/\n");
        Sleep(1500);
        menu_5_1();
        return;
    }
    change_ship_sizes(&ship_sizes, new_number_of_ships);
}

void menu_5_2() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("Enter the new size that is between 3 and 26\n");
    int new_size;
    scanf("%d", &new_size);
    if (new_size < 3 || new_size > 26) {
        printf("Invalid input :/\n");
        Sleep(1500);
        menu_5_2();
        return;
    }
    change_map_size(new_size);
}

void menu_5() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("1. ships\n"
           "2. map size\n");

    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            menu_5_1();
            break;
        case 2:
            menu_5_2();
            break;
        default:
            printf("Invalid input. Choose a valid option.\n");
            menu_5();
            return;
    }
}

void menu_6() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");

    sort_players();
    for (int i = 0; i < number_of_players; ++i)
        printf("%d. %s\twith %d points\n", i + 1, players[i].name, players[i].score);
    printf("\nPress any key to exit\n");
    getch();
}

int menu() {
    system("cls");
    printf("\t      BATTLESHIPS\n\t<developed by Taha Elmi>\n\n");
    printf("1. Play with a Friend\n"
           "2. Play with bot\n"
           "3. Load game\n"
           "4. Load last game\n"
           "5. Settings\n"
           "6. Score Board\n"
           "7. Exit\n");

    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            menu_1();
            return 1;
        case 2:
            menu_2();
            return 2;
        case 3:
            menu_3();
            return 3;
        case 4:
            menu_4();
            return 4;
        case 5:
            menu_5();
            return 3;
        case 6:
            menu_6();
            return 6;
        case 7:
            sort_players();
            save_players();
            update_files();
            return 0;
        default:
            printf("Invalid input. Choose a valid option.\n");
            Sleep(1500);
            return menu();
    }
}