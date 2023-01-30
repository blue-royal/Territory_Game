#ifndef __ARCHER__
#define __ARCHER__

#include "raylib.h"
#include "utils.h"
#include "environment.h"
#include "sprites.h"
#include <vector>

enum class State : int {idle_state, attack_state};

class Archer: public Sprite
{
    private:
        Model archer_model;
        Model halo_model;
        
        Shader shader;
        Environment* env;

      
        Vector3 target;
        float speed;
        bool attack_mode = false;

        float range = pow(5.0f, 2);
        float damage = 5.0f;
        float cool_down = 0.0f;
        float cool_down_time = 2.0f;

        void next_node();

    public:
        Archer();
        Archer(Vector3 start, Teams colour, Environment* environ);
        void update(std::vector<Sprite*> sprites);
        void update_target(Vector3 new_target);
        void attack_move();
        void draw();

        void toggle_attack();
        
        void new_mine_area();
        ~Archer();

};

#endif