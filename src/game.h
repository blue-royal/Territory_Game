#pragma once


#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include <iostream>
#include <vector>

class Game{
    private:
        Camera camera;

        Environment* env;
        std::vector<Sprite*> sprites;
        
        void initialise_game();
        void game_loop();
    public:
        Game();
        void run_game();
        ~Game();
};
