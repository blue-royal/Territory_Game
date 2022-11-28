#include "game.h"
#include <iostream>

int main() {
    InitWindow(800, 500, "Territory");
    std::cout << "start" << std::endl;
    Game territory_game;
    std::cout << "game initialised" << std::endl;
    territory_game.run_game();
    
    return 0;
}