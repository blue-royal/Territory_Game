#include "game.h"
#include <iostream>

int main() {
    InitWindow(800, 500, "Territory");
    
    Game* territory_game = new Game();
    territory_game->run_game();

    delete territory_game;

    return 0;
}
