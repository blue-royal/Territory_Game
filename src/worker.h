#pragma once

#include "raylib.h"
#include "utils.h"
#include "environment.h"
#include "sprites.h"
#include <vector>

class Worker: public Sprite
{
    private:
        Model worker_model;
        Color team;
        Shader shader;
        Environment* env;
        
        Vector3 target;
        float speed;
        

        void next_node();

    public:
        Worker();
        Worker(Vector3 start, Color colour, Environment* environ);
        void update(std::vector<Sprite*> sprites);
        void update_target(Vector3 new_target);
        void draw();
        void new_node_in_region(Vector2 mining_area);
        ~Worker();
};
