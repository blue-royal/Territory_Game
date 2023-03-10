#include "sprites.h"

void Sprite::update(std::vector<Sprite*> sprites){
    std::cout << "override the update function" << std::endl;
}

void Sprite::draw(){
    // default drawing of model
    DrawSphereWires(position, 0.5f, 5, 5, BLACK);
}

void Sprite::is_selected(Vector2 corner1, Vector2 corner2, Camera camera, bool keep_existing){
    // convert the current position of the sprite to the sceen postion
    Vector2 screen_pos = GetWorldToScreen(position, camera);

    // check if sprite is inside the rectangular box selected by player
    // point - rectangle collision
    if(((screen_pos.x > corner1.x && screen_pos.x < corner2.x) || (screen_pos.x < corner1.x && screen_pos.x > corner2.x)) && 
    ((screen_pos.y > corner1.y && screen_pos.y < corner2.y) || (screen_pos.y < corner1.y && screen_pos.y > corner2.y))){
        selected = true;
    } else {
        // check if the current selection should be wiped or kept.
        if(!keep_existing){
            selected = false;
        }
    }
}

void Sprite::draw_health_bar(Camera cam){
    // get screen position of unit
    Vector2 screen_pos = GetWorldToScreen(position, cam);
    // draw background of health bar
    DrawRectangle(screen_pos.x-20.0f, screen_pos.y - 30.0f, 40, 8, CLITERAL(Color){255, 255, 255, 100});
    // draw the colour gradient of health
    DrawRectangleGradientH(screen_pos.x-17.0f, screen_pos.y - 28.0f, 34, 4, CLITERAL(Color){ 230, 41, 55, 150 }, CLITERAL(Color){ 0, 228, 48, 150 });
    // draw the bar that covers up setions of the gradient to indicate health
    DrawRectangle(screen_pos.x-17.0f + (34*(health/full_health)), screen_pos.y - 28.0f, 34 - (34 * (health/full_health)) + 1, 4, CLITERAL(Color){255, 255, 255, 255});

}

void Sprite::decrease_health(float damage){
    health -= damage;
}

// set the initial health of a sprite
void Sprite::set_health(float health){
    // set full health so health bar is a propotion of full health
    full_health = health;
    this->health = health;
}

Sprite::~Sprite(){

}