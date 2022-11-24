#include"worker.h"

Worker::Worker(Color colour){

    team = colour;
    worker_model = LoadModel("assets/models/worker_model/work.glb");
    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    worker_model.materials[0].shader = shader;   
    position = (Vector3){ 3.0f, 1.0f, 2.0f };
    target = (Vector3){ 0.0f, 0.0f, 0.0f };

    speed = 0.1f;
    
}

void Worker::update(Ray mouse_ray){
    // if (IsMouseButtonPressed(0)){
    //     if (ray_sphere_intersection(mouse_ray, position, 0.6f)){
            
    //     }
    // }
    if (IsMouseButtonPressed(1)){
        target = ray_ground_intersection(mouse_ray);
        target.y += 1.0f;
    }
    position = move_to_target(position, target, speed);
}

void Worker::draw(){
    DrawModel(worker_model, position, 0.5f, team);
    DrawModel(worker_model, target, 0.5f, team);

}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
}