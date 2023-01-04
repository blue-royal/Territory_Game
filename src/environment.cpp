#include "environment.h"

Environment::Environment(){

}

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

Vector2 Environment::closest_neutral(Vector2 pos, Vector2 target){
    float x, z;
    unsigned int counter = 0;

    float minimum_score = INFINITY;
    Vector2 current_return = (Vector2){  0.0f, 0.0f };

    for (std::vector<Tiles>::iterator i = grid.begin(); i != grid.end(); i++) {
        if (*i == neutral_tile){
            x = counter%width;
            z = (int)(counter/width);

            Vector2 tile_pos = (Vector2){ x + 0.5f, z + 0.5f };

            float score = Vector2Distance(target, tile_pos) + Vector2Distance(pos, tile_pos);
            if ( score < minimum_score ){
                minimum_score = score;
                current_return = tile_pos;
            }
        }
        counter++;
    }
    return current_return;
}

bool Environment::is_neutral(Vector2 pos){
    if (pos.x < 0.0f || pos.x > width + 1){
        return false;
    }
    
    if (pos.y < 0.0f|| pos.y > height + 1){
        return false;
    }

    // if the tile is an obstical or air tile then it is invalid
    if(grid[static_cast<int>(pos.x) + (static_cast<int>(pos.y) * width)] == neutral_tile){
        return true;
    }
    return false;
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

            all_nodes.push_back(Node((Vector2){x-1, z-1}));
            all_nodes.push_back(Node((Vector2){x+1, z+1}));
            all_nodes.push_back(Node((Vector2){x-1, z+1}));
            all_nodes.push_back(Node((Vector2){x+1, z-1}));

            all_obsticals.push_back((Vector2){x + 0.5f, z + 0.5f});
        }
        counter ++;
    }

  
    // remove duplicates in vector
    for (std::vector<Node>::iterator i = all_nodes.begin(); i < all_nodes.end() - 1; i++) {
        Vector2 vec = i->coords;
        for (std::vector<Node>::iterator j = i + 1; j < all_nodes.end(); j++) {
            if (Vector2Equals(vec, j->coords)){
                all_nodes.erase(j);
            }
        }
    }


    // remove air and obstical tiles
    // remove all nodes that are adjacent to obstical nodes
    Tiles curr_tile, right_tile, left_tile, down_tile, up_tile;

    bool to_remove;
    for (std::vector<Node>::iterator i = all_nodes.begin(); i < all_nodes.end(); i++) {
        to_remove = false;
        curr_tile = grid[i->coords.x + (i->coords.y * width)];
        // if the node is an obstical or air tile remove it        
        if (curr_tile == obstical_tile || curr_tile == air_tile){
            to_remove = true;
        }
        // if the node is adjacent to an obstical or air tile remove it
        left_tile = grid[i->coords.x + 1 + (i->coords.y * width)];
        if (left_tile == obstical_tile){
            to_remove = true;
        }
        right_tile = grid[i->coords.x - 1 + (i->coords.y * width)];
        if (right_tile == obstical_tile){
            to_remove = true;
        }
        up_tile = grid[i->coords.x + ((i->coords.y + 1) * width)];
        if (up_tile == obstical_tile){
            to_remove = true;
        }
        down_tile = grid[i->coords.x + ((i->coords.y - 1) * width)];
        if (down_tile == obstical_tile){
            to_remove = true;
        }

        if(to_remove){
            // erase the node and then go back a memory address
            all_nodes.erase(i--);
        }
    }

    all_nodes.push_back(Node());
    all_nodes.push_back(Node());
    counter = 0;
    for (std::vector<Node>::iterator j = all_nodes.begin(); j < all_nodes.end(); j++) {
        j->index = counter;
        counter ++;
        std::cout << j->coords.x << ", " << j->coords.y << std::endl;
    }
    graph_size = all_nodes.size();
    for (unsigned int i = 0; i < graph_size-2; i++){
        for (unsigned int j = 0; j < graph_size-2; j++){
            if (valid_route(all_nodes[i].coords, all_nodes[j].coords) && valid_route(all_nodes[j].coords, all_nodes[i].coords)){
                nav_graph.push_back(Vector2Distance(all_nodes[i].coords, all_nodes[j].coords));
            }else{
                nav_graph.push_back(-1);
            }
        }
        nav_graph.push_back(-1);
        nav_graph.push_back(-1);
    }
    for (unsigned int i = 0; i < graph_size; i++){
        nav_graph.push_back(-1);
        nav_graph.push_back(-1);
    }

    for (unsigned int i = 0; i < graph_size; i++){
        for (unsigned int j = 0; j < graph_size; j++){
            std::cout << nav_graph[(i*graph_size) + j] << ", ";
        }
    }
}

bool Environment::valid_route(Vector2 start, Vector2 end){
    float dx, dy, x, y;
    int len;

    dx = end.x - start.x;
    dy = end.y - start.y;
    
    x = start.x;
    y = start.y;
    
    if (abs(dy) < abs(dx)){
        len = abs(dx);
        dy = dy / abs(dx);
        dx = dx / abs(dx);
    } else {
        len = abs(dy);
        dx = dx / abs(dy);
        dy = dy / abs(dy);
    }
    for (int i = 0; i <= len; i ++){
        if (!valid_target((Vector2){ x, y })){
            return false;
        }
        y += dy;
        x += dx;
    }

    return true;
}

Vector2 Environment::gen_route(Vector2 start, Vector2 end){

    // if can travel directly to end node - then the function is complete
    if (valid_route(start, end)){
        return end;
    }
    Node begin = Node(start);
    Node finish = Node(end);
    // otherwise add the start and end node to the graph
    for (unsigned int i = 0; i < graph_size-2; i++){
        if (valid_route(start, all_nodes[i].coords) && valid_route(all_nodes[i].coords, start)){
            nav_graph[(graph_size-2)*graph_size + i] = Vector2Distance(start, all_nodes[i].coords);
            nav_graph[i*graph_size + (graph_size-2)] = Vector2Distance(start, all_nodes[i].coords);
        }else{
            nav_graph[(graph_size-2)*graph_size + i] = -1;
            nav_graph[i*graph_size + (graph_size-2)] = -1;
        }
    
        if (valid_route(end, all_nodes[i].coords) && valid_route(all_nodes[i].coords, end)){
            nav_graph[(graph_size-1)*graph_size + i] = Vector2Distance(end, all_nodes[i].coords);
            nav_graph[i*graph_size + (graph_size-1)] = Vector2Distance(end, all_nodes[i].coords);
        }else{
            nav_graph[(graph_size-1)*graph_size + i] = -1;
            nav_graph[i*graph_size + (graph_size-1)] = -1;
        }

        nav_graph[(graph_size-1) * graph_size + (graph_size - 1)] = -1;
        nav_graph[(graph_size-1) * graph_size + (graph_size - 1)] = -1;
        nav_graph[(graph_size-1) * graph_size + (graph_size - 1)] = -1;
        nav_graph[(graph_size-1) * graph_size + (graph_size - 1)] = -1;
    }

    begin.index = graph_size - 2;
    finish.index = graph_size - 1;
    all_nodes[graph_size - 2] = begin;
    all_nodes[graph_size - 1] = finish;



        // apply A* algorithm
        std::vector<unsigned int> open_set;

        for (unsigned int i = 0; i < graph_size; i++) {
            all_nodes[i].f_score = INFINITY;
            all_nodes[i].g_score = INFINITY;
        }

        open_set.push_back(graph_size - 2);
        all_nodes[open_set[0]].g_score = 0;
        all_nodes[open_set[0]].f_score = all_nodes[open_set[0]].heuristic(end);

        while(!open_set.empty()){
            // find element in the openset witrh lowest f score 
            unsigned int min_f_score_index = 0;
            for(unsigned int i = 1; i < open_set.size(); i++){
                if (all_nodes[open_set[i]].f_score < all_nodes[open_set[min_f_score_index]].f_score){
                    min_f_score_index = i;
                }
            }


            Node current = all_nodes[open_set[min_f_score_index]];
            if (Vector2Equals(current.coords, end)){
                //reconstruct path and done
                int temp = current.index;
                while (all_nodes[all_nodes[temp].previous].previous != -1){

                    temp = all_nodes[temp].previous;
                    
                }

                return all_nodes[temp].coords;
            }
            open_set.erase(open_set.begin() + min_f_score_index);

            for (unsigned int i = 0; i < graph_size; i++){
                if (nav_graph[current.index * graph_size + i] != -1 && nav_graph[current.index * graph_size + i] != 0){
                    float tentative_g = current.g_score + nav_graph[(current.index * graph_size) + i];
                    if (tentative_g < all_nodes[i].g_score){
                        all_nodes[i].previous = current.index;
                        all_nodes[i].g_score = tentative_g;
                        all_nodes[i].f_score = tentative_g + all_nodes[i].heuristic(end);

                        bool found = false;
                        for (unsigned int j = 0; j < open_set.size(); j++){
                            if(all_nodes[i].index == all_nodes[open_set[j]].index){
                                found = true;
                            }
                        }
                        if (!found){
                            open_set.push_back(i);
                        }
                    }
                }
            }
        }
    std::cout << "now way through" << std::endl;
    return end;
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
    switch (grid[static_cast<int>(target.x) + (static_cast<int>(target.y) * width)])
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

void Environment::update_tile(Vector2 pos, Teams team){
    if (team == Teams::red_team){
        grid[static_cast<int>(pos.x) + (static_cast<int>(pos.y) * width)] = red_tile;
    } else if (team == Teams::blue_team){
        grid[static_cast<int>(pos.x) + (static_cast<int>(pos.y) * width)] = blue_tile;
    }
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
    
}

Environment::~Environment(){
    UnloadShader(shader);
    UnloadModel(blue_block);
    UnloadModel(red_block);
    UnloadModel(neutral_block);
    UnloadModel(obstical_block);
}

Node::Node(){

}

Node::Node(Vector2 point){
    coords = point;
}

float Node::heuristic(Vector2 end){
    // euclidian distance from current node to end
    return Vector2Distance(coords, end);
}