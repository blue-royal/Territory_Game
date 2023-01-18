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
    private:
        Camera camera;

        Environment* env;
        
        bool server_or_client;

        Vector3 ground_intersect;
        Vector2 corner1;
        Vector2 corner2;
        Vector2 corner3;
        Vector2 corner4;

    protected:
        std::vector<Sprite*> sprites;
        void initialise_game();
        void event_update_draw();

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