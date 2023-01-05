#ifndef __GAME__
#define __GAME__

#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include "network.h"
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

#endif