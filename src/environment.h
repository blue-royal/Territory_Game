#ifndef __SPRITES__
#define __SPRITES__

#include "utils.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>

#include "raylib.h"

struct Grid_point
{
    int x;
    int y;
};

class Environment
{
    private:
        enum Tiles { neutral_tile, red_tile, blue_tile, obstical_tile, air_tile  };
        unsigned int width;
        unsigned int height;
        std::vector<Tiles> grid;
        Model red_block;
        Model blue_block; 
        Model neutral_block;
        Model obstical_block;
        Shader shader;

        

    public:
        Environment(char *level_path);
        void load_level(char *level_path);
        bool valid_target(Vector2 target);
        void update_tile();
        void create_graph();
        void draw();
        ~Environment();
        
};

#endif