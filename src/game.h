#ifndef __GAMECLS__
#define __GAMECLS__

#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include <iostream>

class Game{
    private:
        Camera camera;

        Environment* env;
        Worker* work;
        
        void initialise_game();
        void game_loop();
    public:
        Game();
        void run_game();
        ~Game();
};

#endif
