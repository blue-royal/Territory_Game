#include"worker.h"

Worker::Worker(Color colour){

    team = colour;

    worker_model = LoadModel("assets/models/worker_model/work.glb");
    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    worker_model.materials[0].shader = shader;   
    position = (Vector3){ 3.0f, 1.0f, 2.0f };
    target = (Vector3){ 0.0f, 0.0f, 0.0f };

    speed = 0.05f;
    
}

void Worker::update(){

    position = move_to_target(position, target, speed);
}

void Worker::update_target(Vector3 new_target){
    target = new_target;
}

void Worker::draw(){
    DrawModel(worker_model, position, 0.3f, team);
    DrawModel(worker_model, target, 0.5f, team);

}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
}
