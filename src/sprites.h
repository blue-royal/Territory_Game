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
        explicit Sprite(Sprite_Type NType) : Type(NType) {};
        virtual void update(std::vector<Sprite*> sprites);
        virtual void draw();
        void is_selected(Vector3 corner1, Vector3 corner2, Vector3 corner3, Vector3 corner4);
        ~Sprite();

        Vector3 position;
        bool selected = false;

        bool reached_goal;
        Vector3 goal;
        Sprite_Type Type;
};

#endif
