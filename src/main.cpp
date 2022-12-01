#include "game.h"
#include <iostream>

int main() {
    InitWindow(800, 500, "Territory");
    Game territory_game;
    territory_game.run_game();
    
    return 0;
}
