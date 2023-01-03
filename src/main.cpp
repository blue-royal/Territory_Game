#include "game.h"
#include <iostream>

#define ASIO_STANDALONE
#include "asio.hpp"

int main() {
    InitWindow(1600, 1000, "Territory");
    
    Game* territory_game = new Game();
    territory_game->run_game();

    delete territory_game;

    return 0;
}
