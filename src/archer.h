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
        Teams team;
        Shader shader;
        Environment* env;

      
        Vector3 target;
        float speed;

        float health = 200.0f;

        void next_node();

    public:
        Archer();
        Archer(Vector3 start, Teams colour, Environment* environ);
        void update(std::vector<Sprite*> sprites);
        void update_target(Vector3 new_target);
        void attack_move();
        void draw();

        void decrease_health(float damage);
        
        void new_mine_area();
        ~Archer();
};

class c_archer : public Sprite 
{

};

#endif