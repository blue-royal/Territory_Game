#include "game.h"
#include "menu.h"
#include <iostream>
#include <chrono>
#include <thread>

#define SERVER 0
#define CLIENT 1

int main() {

    InitWindow(800, 500, "Territory");

    int res = run_menu();
    if (res == SERVER)
    {
        S_Game* territory_game = new S_Game();
        territory_game->run_game();

        delete territory_game;
    } 
    else if (res == CLIENT)
    {
        C_Game* territory_game = new C_Game();
        territory_game->run_game();

        delete territory_game;
    }
    return 0;
}
