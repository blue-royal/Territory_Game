#include "game.h"
#include "menu.h"
#include <iostream>
#include <chrono>
#include <thread>

#define SERVER 0
#define CLIENT 1
#define LOCAL 2

int main() {

    // Start a raylib window of size 800 x 500 and title
    InitWindow(800, 500, "Territory");

    // Load the start menu to determine which instance to run
    int res = run_menu();
    if (res == SERVER)
    {
        // run server instance of game
        S_Game* territory_game = new S_Game();
        territory_game->run_game();

        delete territory_game;
    } 
    else if (res == CLIENT)
    {
        // run client instance of game
        C_Game* territory_game = new C_Game();
        territory_game->run_game();

        delete territory_game;
    }
    else if (res == LOCAL)
    {
        // run local instance of game
        L_Game* territory_game = new L_Game();
        territory_game->run_game();

        delete territory_game;
    }
    return 0;
}
