#include "game.h"
#include "menu.h"
#include <iostream>

#define SERVER true
#define CLIENT false

int main() {

    InitWindow(1600, 1000, "Territory");

    bool res = run_menu();
    if (res == SERVER)
    {
        Game* territory_game = new Game();
        territory_game->run_game();

        delete territory_game;
    } 
    else if (res == CLIENT){
        Game* territory_game = new Game();
        territory_game->run_game();

        delete territory_game;
    }

    return 0;
}
