#pragma once

#include "raylib.h"
#include "environment.h"
#include <iostream>

enum class Sprite_Type : int {worker_unit, army_unit};

class Sprite
{
    public:
        Sprite() = default;
        explicit Sprite(Sprite_Type NType) : Type(NType) {};
        virtual void update(std::vector<Sprite*> sprites);
        virtual void update_target(Vector3 new_target);
        virtual void draw();
        void is_selected(Vector3 corner1, Vector3 corner2);
        ~Sprite();

        Vector3 position;
        bool selected = false;

        bool reached_goal;
        Vector3 goal;
        Sprite_Type Type;
};
