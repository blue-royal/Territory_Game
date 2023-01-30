#include "sprites.h"

// Sprite::Sprite(){
//     halo_model = LoadModel("assets/models/selected_halo/halo.obj");
//     shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
//     halo_model.materials[0].shader = shader;   

//     std::cout << " it is running" << std::endl;
// }

void Sprite::update(std::vector<Sprite*> sprites){
    std::cout << "override the update function" << std::endl;
}

void Sprite::draw(){
    // default drawing of model
    DrawSphereWires(position, 0.5f, 5, 5, BLACK);
}

void Sprite::is_selected(Vector2 corner1, Vector2 corner2, Camera camera){

    Vector2 screen_pos = GetWorldToScreen(position, camera);


    if(((screen_pos.x > corner1.x && screen_pos.x < corner2.x) || (screen_pos.x < corner1.x && screen_pos.x > corner2.x)) && 
    ((screen_pos.y > corner1.y && screen_pos.y < corner2.y) || (screen_pos.y < corner1.y && screen_pos.y > corner2.y))){
        selected = true;
    } else {
        selected = false;
    }
}

void Sprite::draw_health_bar(Camera cam){
    Vector2 screen_pos = GetWorldToScreen(position, cam);
    DrawRectangle(screen_pos.x-20.0f, screen_pos.y - 30.0f, 40, 8, BLACK);
    DrawRectangleGradientH(screen_pos.x-17.0f, screen_pos.y - 28.0f, 34, 4, RED, GREEN);
    DrawRectangle(screen_pos.x-17.0f + (34*(health/full_health)), screen_pos.y - 28.0f, 34 - (34 * (health/full_health)) + 1, 4, BLACK);

}

void Sprite::decrease_health(float damage){
    health -= damage;
}

void Sprite::set_health(float health){
    full_health = health;
    this->health = health;
}

Sprite::~Sprite(){

}