#include "game.h"
#include <iostream>

int main() {

    InitWindow(1600, 1000, "Territory");
    
    Game* territory_game = new Game();
    territory_game->run_game();

    delete territory_game;

    return 0;
}
