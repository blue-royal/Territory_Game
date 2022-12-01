#include "environment.h"

Environment::Environment(char *level_path){
    load_level(level_path);
    std::cout << "level loaded" << std::endl;
    red_block = LoadModel("assets/models/red_block/red.obj");
    std::cout << "red loaded" << std::endl;
    blue_block = LoadModel("assets/models/blue_block/blue.obj");
    neutral_block = LoadModel("assets/models/neutral_block/neutral.obj");
    obstical_block = LoadModel("assets/models/obstical_block/obstical.obj");

    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));

    red_block.materials[0].shader = shader;   
    blue_block.materials[0].shader = shader;   
    neutral_block.materials[0].shader = shader;
    obstical_block.materials[0].shader = shader;  

    create_graph();

}

void Environment::load_level(char *level_path){

    std::vector<std::string> tiles = read_file(level_path);

    height = tiles.size();
    width = tiles[0].size();

    for (std::vector<std::string>::iterator i = tiles.begin(); i != tiles.end(); i++) {
        std::string layer = *i;
        for (std::string::size_type j = 0; j < layer.size(); j++) {

            switch (layer[j])
            {
            case '0':
                grid.push_back(air_tile);
                break;
            case '1':
                grid.push_back(neutral_tile);
                break;
            case '2':
                grid.push_back(blue_tile);
                break;
            case '3':
                grid.push_back(red_tile);
                break;
            case '4':
                grid.push_back(obstical_tile);
                break;
            default:
                std::cout << "Couldn't find tile type" << std::endl;
                break;
            }
        }
    }
}

void Environment::create_graph(){
    
    unsigned int counter = 0;
    float x;
    float z;
    // vector of all possible nodes is made from all nodes diagnonal to an obstical tile
    for (std::vector<Tiles>::iterator i = grid.begin(); i != grid.end(); i++) {
        if (*i == obstical_tile){
            x = counter%width;
            z = (int)(counter/width);

            all_nodes.push_back((Vector2){x+1, z+1});
            all_nodes.push_back((Vector2){x-1, z-1});
            all_nodes.push_back((Vector2){x-1, z+1});
            all_nodes.push_back((Vector2){x+1, z-1});
        }
        counter ++;
    }

  
//     // remove duplicates in vector
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end() - 1; i++) {
        Vector2 vec = *i;
        for (std::vector<Vector2>::iterator j = i + 1; j < all_nodes.end(); j++) {
            if (Vector2Equals(vec, *j)){
                all_nodes.erase(j);
            }
        }
    }


    // remove air and obstical tiles
    // remove all nodes that are adjacent to obstical nodes
    Tiles curr_tile;
    Tiles up_tile;
    Tiles down_tile;
    Tiles left_tile;
    Tiles right_tile;
    bool to_remove;
    std::cout << all_nodes.size() << std::endl;
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end(); i++) {
        to_remove = false;
        curr_tile = grid[(*i).x + ((*i).y * width)];
        // if the node is an obstical or air tile remove it        
        if (curr_tile == obstical_tile || curr_tile == air_tile){
            to_remove = true;
        }
        // if the node is adjacent to an obstical or air tile remove it
        left_tile = grid[(*i).x + 1 + ((*i).y * width)];
        if (left_tile == obstical_tile){
            to_remove = true;
        }
        right_tile = grid[(*i).x - 1 + ((*i).y * width)];
        if (right_tile == obstical_tile){
            to_remove = true;
        }
        up_tile = grid[(*i).x + (((*i).y + 1) * width)];
        if (up_tile == obstical_tile){
            to_remove = true;
        }
        down_tile = grid[(*i).x + (((*i).y - 1) * width)];
        if (down_tile == obstical_tile){
            to_remove = true;
        }

        if(to_remove){
            // erase the node and then go back a memory address
            all_nodes.erase(i--);
        }
    }

    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end() - 1; i++) {
        Vector2 vec = *i;
        for (std::vector<Vector2>::iterator j = i + 1; j < all_nodes.end(); j++) {
            std::cout << valid_route(*i, *j) << std::endl;
        }
    }

}

bool Environment::valid_route(Vector2 &start, Vector2 &end){
    // bresmans line algorithm
    int dx, dy, p, x, y;

    if (start.x > end.x){
        Vector2 temp = start;
        start = end;
        end = temp;
    }

    std::cout << "start: " << start.x << ", " << start.y << std::endl;
    std::cout << "end: " << end.x << ", " << end.y << std::endl;


    dx = end.x - end.y;
    dy = end.y - start.y;
    
    x=start.x;
    y=start.y;
    
    p = 2 * (dy - dx);
    
    while(x < end.x){
        if(p>=0){
            // std::cout << x << ", " << y << std::endl;
            if (grid[x + (y*width)] == obstical_tile){
                return false;
            }
            y=y+1;
            p=p+2*dy-2*dx;
        }else{
            // std::cout << x << ", " << y << std::endl;
            if (grid[x + (y*width)] == obstical_tile){
                return false;
            }
            p=p+2*dy;
        }
        x++;
    }
    return true;
}


bool Environment::valid_target(Vector2 target){
    if (target.x < 0.0f || target.x > width + 1){
        return false;
    }
    
    if (target.y < 0.0f|| target.y > height + 1){
        return false;
    }

    switch (grid[static_cast<int>(target.x) + ((int)target.y * width)])
    {
    case obstical_tile:
        return false;
        break;
    case air_tile:
        return false;
        break;
    default:
        return true;
        break;
    }
}

void Environment::update_tile(){
    
}

void Environment::draw(){
    unsigned int counter = 0;
    for (std::vector<Tiles>::iterator i = grid.begin(); i != grid.end(); i++) {
        float x = counter%width;
        float z = (int)(counter/width);
        switch (*i)
        {
        case blue_tile:
            DrawModel(blue_block, (Vector3){x, -1.0f, z }, 0.5f, BLUE);
            break;
        case red_tile:
            DrawModel(red_block, (Vector3){x, -1.0f, z }, 0.5f, RED);
            break;
        case neutral_tile:
            DrawModel(neutral_block, (Vector3){x, -1.0f, z }, 0.5f, WHITE);
            break;
        case obstical_tile:
            DrawModel(obstical_block, (Vector3){x, -1.0f, z }, 0.5f, WHITE);
            break;
        case air_tile:
            break;        
        default:
            std::cout << "couldn't detect tile type" << *i << std::endl;
            break;
        }
        
        counter++;
    }
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i != all_nodes.end(); i++) {
        DrawModel(blue_block, (Vector3){(*i).x, 0.0f, (*i).y }, 0.5f, WHITE);
    }
}

Environment::~Environment(){
    UnloadShader(shader);
    UnloadModel(blue_block);
    UnloadModel(red_block);
    UnloadModel(neutral_block);
}
