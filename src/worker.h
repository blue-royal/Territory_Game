#pragma once

#include "raylib.h"
#include "utils.h"
#include "environment.h"
#include "sprites.h"
#include <vector>

class Worker: public Sprite
{
    private:

        enum State { idle_state, working_state  };

        Model worker_model;
        Teams team;
        Shader shader;
        Environment* env;

        State current_state;

        float working_time;        
        Vector3 target;
        float speed;
        Vector2 mine_area;

        
        void new_node_in_region();
        void next_node();

    public:
        Worker();
        Worker(Vector3 start, Teams colour, Environment* environ);
        void update(std::vector<Sprite*> sprites);
        void update_target(Vector3 new_target);
        void draw();
        
        void new_mine_area();
        ~Worker();
};
