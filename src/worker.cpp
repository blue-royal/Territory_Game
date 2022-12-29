#include"worker.h"

Worker::Worker(){

}

Worker::Worker(Vector3 start, Color colour, Environment* environ){
    // load model shader and relevant positions for the worker
    team = colour;
    env = environ;

    worker_model = LoadModel("assets/models/worker_model/work.glb");
    Shader shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    worker_model.materials[0].shader = shader;   
    position = start;
    target = start;
    goal = start;
    reached_goal = true;

    speed = 0.04f;
    
}

void Worker::update(std::vector<Sprite*> sprites){
    //if the player is at the target get the next target
    if(!reached_goal){
        reached_goal = reached_target(position, goal);
    }
    
    if (reached_target(position, target) && !reached_goal){
        update_target(goal);
    }

    // move the player towards the target by their speed
    // if the player hasn't reached the goal
    if (!reached_goal){
        // check any collision that the agent is making with other agents of the obsticals

        // look ahead to see if there are any obsticals in the way and adjust accordingly
        const float MAX_SEE_AHEAD = 0.5f;
        const float MAX_AVOIDANCE_FORCE = 1.0f;
        Vector3 ahead = move_to_target(position, target, MAX_SEE_AHEAD);
        Vector3 avoidance_force;
        if (! env->valid_target(vec3_to_vec2(ahead))){
            avoidance_force = (Vector3){ ahead.x - ((int)ahead.x + 0.5f), 0.0f, ahead.z - ((int)ahead.z + 0.5f) };
            avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
            position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
            
        } else {
        
            bool blockage = false;
            for (std::vector<Sprite*>::iterator i = sprites.begin(); i != sprites.end(); i++){
                if (circle_circle_intersection(vec3_to_vec2(position), vec3_to_vec2((*i)->position), 0.3f, 0.3f)){
                    blockage = true;
                    avoidance_force = (Vector3){ position.x - (*i)->position.x, 0.0f, position.z - (*i)->position.z };
                    avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                    position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                    if ((*i)->reached_goal && Vector3DistanceSqr(goal, (*i)->goal) < 1.0f){
                        reached_goal = true;
                    }   
                } else {
                    if (point_circle_intersection(vec3_to_vec2(ahead), vec3_to_vec2((*i)->position), 0.5f )){
                         if (Vector3DistanceSqr(goal, (*i)->goal) > 1.0f){
                            blockage = true;
                            avoidance_force = (Vector3){ ahead.x - (*i)->position.x, 0.0f, ahead.z - (*i)->position.z };
                            avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                            position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                         }
                    }
                }


            }
            if (!blockage){
                position = move_to_target(position, target, speed);
            }
        }    
    }
}

void Worker::update_target(Vector3 new_target){
    goal = new_target;
    reached_goal = false;
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

void new_node_in_region(Vector2 mining_area){
    Vector2 home = (Vector2){ 3.5f, 3.5f };
    // for 
}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
}
