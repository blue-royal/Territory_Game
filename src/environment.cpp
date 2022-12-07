#include "environment.h"

Environment::Environment(char *level_path){
    load_level(level_path);
    red_block = LoadModel("assets/models/red_block/red.obj");
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

    // compare each character in text file to build up level vectorr
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

            all_nodes.push_back((Vector2){x-1, z-1});
            all_nodes.push_back((Vector2){x+1, z+1});
            all_nodes.push_back((Vector2){x-1, z+1});
            all_nodes.push_back((Vector2){x+1, z-1});
        }
        counter ++;
    }

  
    // remove duplicates in vector
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
    Tiles curr_tile, right_tile, left_tile, down_tile, up_tile;

    bool to_remove;
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end(); i++) {
        to_remove = false;
        curr_tile = grid[i->x + (i->y * width)];
        // if the node is an obstical or air tile remove it        
        if (curr_tile == obstical_tile || curr_tile == air_tile){
            to_remove = true;
        }
        // if the node is adjacent to an obstical or air tile remove it
        left_tile = grid[i->x + 1 + (i->y * width)];
        if (left_tile == obstical_tile){
            to_remove = true;
        }
        right_tile = grid[i->x - 1 + (i->y * width)];
        if (right_tile == obstical_tile){
            to_remove = true;
        }
        up_tile = grid[i->x + ((i->y + 1) * width)];
        if (up_tile == obstical_tile){
            to_remove = true;
        }
        down_tile = grid[i->x + ((i->y - 1) * width)];
        if (down_tile == obstical_tile){
            to_remove = true;
        }

        if(to_remove){
            // erase the node and then go back a memory address
            all_nodes.erase(i--);
        }
    }

    // populate the matrix for pathfinding
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end(); i++) {
        Vector2 vec = *i;
        for (std::vector<Vector2>::iterator j = all_nodes.begin(); j < all_nodes.end(); j++) {
            if (valid_route(vec, *j)){
                nav_graph.push_back(Vector2Distance(vec, *j));
            }else{
                nav_graph.push_back(0.0f);
            }
        }
        // add space for start and end nodes in graph
        nav_graph.push_back(0.0f);
        nav_graph.push_back(0.0f);
    }
    // two additional rows for start and end nodes
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end() + 2; i++) {
        nav_graph.push_back(0.0f);
        nav_graph.push_back(0.0f);
    }

    unsigned int mat_size = all_nodes.size() + 2;
    for (int i = 0; i < mat_size; i++){
        for (int j = 0; j < mat_size; j++){
            std::cout << nav_graph[i*width + j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}

bool Environment::valid_route(Vector2 start, Vector2 end){
    // bresmans line algorithm to detect if two nodes can be connected by a straight line
    int dx, dy, p, x, y;

    if (start.x > end.x){
        Vector2 temp = start;
        start = end;
        end = temp;
    }

    dx = end.x - start.x;
    dy = end.y - start.y;
    
    x = start.x;
    y = start.y;
    
    p = 2 * (dy - dx);
    
    // for all points along the line check if they contain an obstruction e.g. an obstical or air tile
    while(x <= end.x){
        if (!valid_target((Vector2){ static_cast<float>(x), static_cast<float>(y) })){
            return false;
        }
        if(p >= 0){
            y++;
            p += 2 * (dy - dx);
        }else{
            p += 2 * dy;
        }
        x++;
    }
    return true;
}

Vector2 Environment::gen_route(Vector2 start, Vector2 end){

    // if can travel directly to end node - then the function is complete
    if (valid_route(start, end)){
        return end;
    }else{
        // add start and end nodes to nav_graph
        unsigned int mat_size = all_nodes.size() + 2;
        unsigned int counter = 0;
        for (std::vector<Vector2>::iterator i = all_nodes.begin(); i < all_nodes.end(); i++) {
            if (valid_route(*i, start)){
                nav_graph[((mat_size - 2) * mat_size) + counter] = Vector2Distance(*i, start);
                nav_graph[((counter + 1) * mat_size) - 2] = Vector2Distance(*i, start);
            }else{
                nav_graph[((mat_size - 2) * mat_size) + counter] = 0.0f;
                nav_graph[((counter + 1) * mat_size) - 2] = 0.0f;
            }
            if (valid_route(*i, end)){
                nav_graph[((mat_size - 1) * mat_size) + counter] = Vector2Distance(*i, end);
                nav_graph[((counter + 1) * mat_size) - 1] = Vector2Distance(*i, end);
            }else{
                nav_graph[((mat_size - 1) * mat_size) + counter] = 0.0f;
                nav_graph[((counter + 1) * mat_size) - 1] = 0.0f;
            }
            counter ++;
        }

        for (int i = 0; i < mat_size; i++){
            for (int j = 0; j < mat_size; j++){
                std::cout << nav_graph[i*width + j] << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // apply A* algorithm
        // return next target node
        return end;
    }
}

bool Environment::valid_target(Vector2 target){
    // if the target is off the grid then it is not valid
    if (target.x < 0.0f || target.x > width + 1){
        return false;
    }
    
    if (target.y < 0.0f|| target.y > height + 1){
        return false;
    }

    // if the tile is an obstical or air tile then it is invalid
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

    // tempororily draw all nodes in the nav mesh for debugging
    for (std::vector<Vector2>::iterator i = all_nodes.begin(); i != all_nodes.end(); i++) {
        DrawModel(blue_block, (Vector3){i->x, 0.0f, i->y }, 0.5f, WHITE);
    }
}

Environment::~Environment(){
    UnloadShader(shader);
    UnloadModel(blue_block);
    UnloadModel(red_block);
    UnloadModel(neutral_block);
    UnloadModel(obstical_block);
}
