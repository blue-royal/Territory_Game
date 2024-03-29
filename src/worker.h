#ifndef __WORKER__
#define __WORKER__

#include "raylib.h"
#include "utils.h"
#include "environment.h"
#include "sprites.h"
#include <vector>
#include <iostream>

class Worker: public Sprite
{
    private:

        enum class State : int { idle_state, working_state  };

        Model worker_model;
        Teams team;
        Shader shader;
        Environment* env;

        Model halo_model;

        State current_state;

        float working_time;        
        Vector3 target;
        float speed;
        Vector2 mine_area;

        
        void new_node_in_region();
        void next_node();

        float health = 100.0f;

    public:
        Worker();
        Worker(Vector3 start, Teams colour, Environment* environ);
        void update(std::vector<Sprite*> sprites);
        void update_target(Vector3 new_target);
        void draw();

        void decrease_health(float damage);
        
        void new_mine_area();
        ~Worker();


};

class c_Worker : public Sprite
{

};

#endif