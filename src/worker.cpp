#include"worker.h"

Worker::Worker(){

}

Worker::Worker(Color colour, Environment* environ){
    // load model shader and relevant positions for the worker
    team = colour;
    env = environ;

    worker_model = LoadModel("assets/models/worker_model/work.glb");
    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    worker_model.materials[0].shader = shader;   
    position = (Vector3){ 3.0f, 1.0f, 2.0f };
    target = (Vector3){ 3.0f, 1.0f, 3.0f };

    speed = 0.05f;
    
}

void Worker::update(){
    //if the player is at the target get the next target
    if (reached_target(position, target)){
        update_target(goal);
    }
    // move the player towards the target by their speed

    position = move_to_target(position, target, speed);
}

void Worker::update_target(Vector3 new_target){
    goal = new_target;
    Vector2 next_target = env->gen_route(vec3_to_vec2(position), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Worker::next_node(){
    Vector2 next_target = env->gen_route(vec3_to_vec2(target), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Worker::draw(){
    DrawModel(worker_model, position, 0.3f, team);
}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
}
