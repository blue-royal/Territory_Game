#ifndef GAME__
#define GAME__

#include "raylib.h"
#include "environment.h"
#include "worker.h"
#include "archer.h"
#include "tower.h"
#include "network.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define WORKER_COST 20.0f
#define ARCHER_COST 40.0f

enum class Network_Headers : int {null, add_worker, delete_sprite, add_archer, update_sprites, update_terrain};

class Game{
    protected:
        void initialise_game();
        void update_draw();
        void events(Teams team);
        void delete_sprite(unsigned int index);

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

        Vector3 spawn = Vector3 {14.0f, 0.0f, 4.0f};
        Vector3 other_spawn = Vector3 {4.0f, 0.0f, 14.0f};

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

        Vector3 spawn = Vector3 {4.0f, 0.0f, 14.0f};
        Vector3 other_spawn = Vector3 {14.0f, 0.0f, 4.0f};
};

class L_Game: public Game
{
    public:
        L_Game() = default;
        void run_game();
};

#endif