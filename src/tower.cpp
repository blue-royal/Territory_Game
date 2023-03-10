#include "tower.h"

Tower::Tower() : Sprite(Sprite_Type::tower_unit, Teams::neutral_team) {  }

Tower::Tower(Vector3 start, Teams colour) : Sprite(Sprite_Type::tower_unit, colour){
    // setup main data for the model to be rendered
    team = colour;

    tower_model = LoadModel("assets/models/tower/tower_model.obj");   
    shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    
    tower_model.materials[0].shader = shader; 

    // initialse postion
    position = start;

    // initiate health
    set_health(1000.0f);
}

// nothing to update
void Tower::update(std::vector<Sprite*> sprites){

}

 void Tower::draw(){
    // draw the model for either teams model
    if (team == Teams::red_team){
        DrawModel(tower_model, position, 0.5f, RED);
    } else if (team == Teams::blue_team){
        DrawModel(tower_model, position, 0.5f, BLUE);
    }
 }

 Tower::~Tower(){
    // unload relevant data
    UnloadShader(shader);
    UnloadModel(tower_model);
 }