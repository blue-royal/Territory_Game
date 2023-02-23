#ifndef TOWER__
#define TOWER__

#include "raylib.h"
#include "utils.h"
#include "sprites.h"
#include <vector>
#include <iostream>

class Tower: public Sprite
{

    private:
        Model tower_model;
        Shader shader;

    public:
        Tower();
        Tower(Vector3 start, Teams colour);
        void update(std::vector<Sprite*> sprites);
        void draw();
        ~Tower();
};

#endif