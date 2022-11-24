#ifndef __SPRITES__
#define __SPRITES__

#include "utils.h"

#include <string>
#include <vector>

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
        Shader shader;

        void load_level(char *level_path);

    public:
        Environment(char *level_path);
        void update_tile();
        void draw();
        ~Environment();
        
};

#endif