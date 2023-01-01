#pragma once

#include "utils.h"

#include <string>
#include <vector>
#include <iostream>

#include "raylib.h"

class Node
{
    public:
        Node();
        Node(Vector2 point);

        Vector2 coords;
        float f_score = INFINITY;
        float g_score = INFINITY;
        unsigned int index;
        int previous = -1;
        float heuristic(Vector2 end);



};

class Environment
{
    private:
        // all tile types
        enum Tiles { neutral_tile, red_tile, blue_tile, obstical_tile, air_tile  };
        // grid width and height
        unsigned int width;
        unsigned int height;
        // linear grid of all tile types
        std::vector<Tiles> grid;
        // all block type models
        Model red_block;
        Model blue_block; 
        Model neutral_block;
        Model obstical_block;
        Shader shader;
        // navigation nodes
        unsigned int graph_size;
        std::vector<Node> all_nodes;
        std::vector<Vector2> all_obsticals;
        std::vector<float> nav_graph;

        
    public:
        Environment();
        Environment(char *level_path);
        void load_level(char *level_path);
        bool valid_target(Vector2 target);
        bool valid_route(Vector2 start, Vector2 end);
        Vector2 gen_route(Vector2 start, Vector2 end);
        Vector2 closest_neutral(Vector2 pos, Vector2 target);
        bool is_neutral(Vector2 pos);
        void update_tile(Vector2 pos, Teams team);
        void create_graph();
        void draw();
        ~Environment();
        
};
