#ifndef __SPRITES__
#define __SPRITES__

#include "utils.h"

#include <string>
#include <vector>
#include <iostream>

#include "raylib.h"


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
        std::vector<Vector2> all_nodes;

        

    public:
        Environment(char *level_path);
        void load_level(char *level_path);
        bool valid_target(Vector2 target);
        bool valid_route(Vector2 &start, Vector2 &end);
        void update_tile();
        void create_graph();
        void draw();
        ~Environment();
        
};

#endif
