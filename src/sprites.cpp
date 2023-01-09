#include "sprites.h"

void Sprite::update(std::vector<Sprite*> sprites){
    std::cout << "override the update function" << std::endl;
}

void Sprite::draw(){
    // default drawing of model
    DrawSphereWires(position, 0.5f, 5, 5, BLACK);
}

void Sprite::is_selected(Vector3 corner1, Vector3 corner2){
    if(((position.x > corner1.x && position.x < corner2.x) || (position.x < corner1.x && position.x > corner2.x)) && 
    ((position.z > corner1.z && position.z < corner2.z) || (position.z < corner1.z && position.z > corner2.z))){
        std::cout << "selected" << std::endl;
        selected = true;
    } else {
        selected = false;
    }
}

Sprite::~Sprite(){

}