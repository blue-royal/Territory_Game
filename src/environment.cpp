#include "environment.h"

Environment::Environment(char *level_path){
    load_level(level_path);

    red_block = LoadModel("assets/models/red_block/red.obj");
    blue_block = LoadModel("assets/models/blue_block/blue.obj");
    neutral_block = LoadModel("assets/models/neutral_block/neutral.obj");

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
            default:
                std::cout << "Couldn't find tile type" << std::endl;
                break;
            }
        }
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
            DrawModel(blue_block, (Vector3){x, 0.0f, z }, 0.5f, WHITE);
            break;
        case red_tile:
            DrawModel(red_block, (Vector3){x, 0.0f, z }, 0.5f, WHITE);
            break;
        case neutral_tile:
            DrawModel(neutral_block, (Vector3){x, 0.0f, z }, 0.5f, WHITE);
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
    UnloadModel(blue_block);
    UnloadModel(red_block);
    UnloadModel(neutral_block);
}