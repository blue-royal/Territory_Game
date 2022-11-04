#include"worker.h"

Worker::Worker(Color colour){

    team = colour;
    worker_model = LoadModel("assets/models/worker_model/worker.obj");
    position = (Vector3){3.0f, 1.0f, 3.0f };

}

void Worker::update(){
    if (IsKeyDown(KEY_W)){
        position.x += 0.1;
    }
    if (IsKeyDown(KEY_S)){
        position.x -= 0.1;
    }
    if (IsKeyDown(KEY_A)){
        position.z += 0.1;
    }
    if (IsKeyDown(KEY_D)){
        position.z -= 0.1;
    }
}

void Worker::draw(){
    DrawModel(worker_model, position, 0.5f, team);

}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
}