#ifndef __GAME__
#define __GAME__

#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include "archer.h"
#include "network.h"
#include <iostream>
#include <vector>
#include <string>


class Game{
    protected:
        void initialise_game();
        void update_draw();
        void events(Teams team);

        Environment* env;
        std::vector<Sprite*> sprites;

        Vector2 corner1;

        Camera camera;

        float points = 0;
        float points_timer = 10.0f;
        float point_countdown = 10.0f;


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

class L_Game: public Game
{
    public:
        L_Game() = default;
        void run_game();
};

#endif