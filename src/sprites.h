#pragma once

#include "raylib.h"
#include "environment.h"
#include <iostream>

enum class Borris : int {worker_unit, army_unit};

class Sprite
{
    public:
        Sprite() = default;
        explicit Sprite(Borris NType) : Type(NType) {};
        virtual void update(std::vector<Sprite*> sprites);
        virtual void update_target(Vector3 new_target);
        virtual void draw();
        virtual void new_mine_area();
        void is_selected(Vector3 corner1, Vector3 corner2);
        static void test();
        ~Sprite();

        Vector3 position;
        bool selected = false;

        bool reached_goal;
        Vector3 goal;
        Borris Type;
};
