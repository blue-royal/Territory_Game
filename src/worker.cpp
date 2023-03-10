#include"worker.h"

Worker::Worker() : Sprite(Sprite_Type::worker_unit, Teams::neutral_team) {  }

Worker::Worker(Vector3 start, Teams colour, Environment* environ) : Sprite(Sprite_Type::worker_unit, colour){
    // load model shader and relevant positions for the worker
    team = colour;
    env = environ;

    worker_model = LoadModel("assets/models/worker_model/work.glb");   
    shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));
    
    worker_model.materials[0].shader = shader; 

    halo_model = LoadModel("assets/models/selected_halo/halo.obj");
    halo_model.materials[0].shader = shader;   

    // initialise relevant values for the worker type
    position = start;
    target = start;
    goal = start;
    reached_goal = true;

    speed = 1.5f;
    working_time = 0.0f;
    set_health(100.0f);
    
}

void Worker::update(std::vector<Sprite*> sprites){
    // update current tile
    if (current_state == State::working_state && env->is_neutral(vec3_to_vec2(position)) && working_time > 2.0f){
        env->update_tile(vec3_to_vec2(position), team);
        working_time = 0.0f;
        new_node_in_region();
    }
    // if if reached a new node to work on, start timer
    if (reached_goal && current_state == State::working_state){
        working_time += GetFrameTime();
    }
    // 
    if ( current_state == State::working_state && !env->is_neutral(vec3_to_vec2(goal))){
        new_node_in_region();
    }

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
            for (Sprite* i : sprites) {
                // determine if unit is in close proximity
                if (circle_circle_intersection(vec3_to_vec2(position), vec3_to_vec2(i->position), 0.3f, 0.3f)){
                    blockage = true;
                    // add a force to avoid colliding with the unit
                    avoidance_force = (Vector3){ position.x - i->position.x, 0.0f, position.z - i->position.z };
                    avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                    position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                    // if they have reache the target and it is similar to current units target
                    if (i->reached_goal && Vector3DistanceSqr(goal, i->goal) < 1.0f && current_state == State::idle_state){
                        // set the goal has been reached
                        reached_goal = true;
                    }   
                } else {
                    if (point_circle_intersection(vec3_to_vec2(ahead), vec3_to_vec2(i->position), 0.5f )){
                         if (Vector3DistanceSqr(goal, i->goal) > 1.0f){
                            blockage = true;
                            avoidance_force = (Vector3){ ahead.x - i->position.x, 0.0f, ahead.z - i->position.z };
                            avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                            position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                         }
                    }
                }
            }
            // if no blockage, move towards the target
            if (!blockage){
                position = move_to_target(position, target, speed);
            }
        }    
    }
}

void Worker::update_target(Vector3 new_target){
    // set up a new goal
    goal = new_target;
    current_state = State::idle_state;
    reached_goal = false;
    Vector2 next_target = env->gen_route(vec3_to_vec2(position), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Worker::next_node(){
    // get the next node in the route to the goal
    Vector2 next_target = env->gen_route(vec3_to_vec2(target), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Worker::draw(){
    // draw the correct teams colour
    if (team == Teams::red_team){
        DrawModel(worker_model, position, 0.3f, RED);
    } else if (team == Teams::blue_team){
        DrawModel(worker_model, position, 0.3f, BLUE);
    }

    // if 'selected' then draw halo model to show that unit is selected
    if (selected){
        DrawModel(halo_model, position, 0.3f, YELLOW);
    }

}

void Worker::new_node_in_region(){
    // get a new tile that can be converted to the teams colour
    update_target(vec2_to_vec3_ground(env->closest_neutral(vec3_to_vec2(position), mine_area)));
    current_state = State::working_state;

}

void Worker::new_mine_area(){
    // setup new area to convert tiles around
    mine_area = vec3_to_vec2(position);
    new_node_in_region();
}

Worker::~Worker(){
    UnloadShader(shader);
    UnloadModel(worker_model);
    UnloadModel(halo_model);
}
