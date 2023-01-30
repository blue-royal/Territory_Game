#ifndef __UTILS__
#define __UTILS__

#include "raylib.h"
#include "raymath.h"
#include "environment.h"
#include <iostream>

enum class Sprite_Type : int {worker_unit, archer_unit};

class Sprite
{

    public:
        Sprite() = default;
        explicit Sprite(Sprite_Type NType, Teams team) : Type(NType), team(team) {};
        virtual void update(std::vector<Sprite*> sprites);
        virtual void draw();
        void is_selected(Vector2 corner1, Vector2 corner2, Camera camera);
        void decrease_health(float damage);
        void draw_health_bar(Camera cam);
        void set_health(float health);
        ~Sprite();

        Vector3 position;
        bool selected = false;

        float full_health;
        float health;

        bool reached_goal;
        Vector3 goal;
        Sprite_Type Type;
        Teams team;
};

#endif
