#ifndef __GAME__
#define __GAME__

#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include "archer.h"
#include "network.h"
#include <iostream>
#include <vector>


class Game{
    protected:
        void initialise_game();
        void event_update_draw();
        void events(Teams team);

        Environment* env;
        std::vector<Sprite*> sprites;
        

        Camera camera;

    public:
        Game();
        ~Game();
};

class C_Game: public Game
{
    public:
        C_Game() = default;
        void run_game();
    private:
        Client client;
        void network();
        void init_network();
};

class S_Game: public Game
{
    public:
        S_Game() = default;
        void run_game();
    private:
        Server server;
        void network();
        void init_network();
};

#endif