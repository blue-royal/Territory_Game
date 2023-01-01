#include "sprites.h"

Sprite::Sprite(){

}

void Sprite::update(std::vector<Sprite*> sprites){
    std::cout << "override the update function" << std::endl;
}

void Sprite::update_target(Vector3 new_target){
    std::cout << "override the update target function" << std::endl;
}

void Sprite::draw(){

}

void Sprite::new_mine_area(){
    std::cout << "workers need this function" << std::endl;
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