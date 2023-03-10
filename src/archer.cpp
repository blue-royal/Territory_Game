#include"archer.h"

Archer::Archer() : Sprite(Sprite_Type::worker_unit, Teams::neutral_team){
}

Archer::Archer(Vector3 start, Teams colour, Environment* environ) : Sprite(Sprite_Type::archer_unit, colour){
    // load model shader and relevant positions for the worker
    team = colour;
    env = environ;

    archer_model = LoadModel("assets/models/archer_model/archer.obj");
    shader = LoadShader(TextFormat("assets/shaders/lighting.vs"), TextFormat("assets/shaders/lighting.fs", 330));

    halo_model = LoadModel("assets/models/selected_halo/halo.obj");
    halo_model.materials[0].shader = shader;   


    archer_model.materials[0].shader = shader;   
    position = start;
    target = start;
    goal = start;
    reached_goal = true;

    speed = 2.3f;
    set_health(200.0f);
    
}

void Archer::update(std::vector<Sprite*> sprites){

    //if the player is at the target get the next target
    if(!reached_goal){
        reached_goal = reached_target(position, goal);
    }
    // if reached the target - define the next target to reach goal
    if (reached_target(position, target) && !reached_goal){
        update_target(goal);
    }
    // if in attack mode then attack the closest unit
    if (attack_mode && cool_down < 0.0f){
        Sprite* closest_sprite = nullptr;
        float closest_dist = INFINITY;
        float dist;
        // determine closest unit
        for (Sprite* i : sprites) {
            if(i->team != team){
                dist = Vector3DistanceSqr(i->position, position);
                if(dist < range && dist < closest_dist){
                    closest_sprite = i;
                    closest_dist = dist;
                }
            }
        }
        if (closest_sprite != nullptr){
            closest_sprite->decrease_health(damage);
            cool_down = cool_down_time;
        }
    }

    if (cool_down >= 0.0f){
        cool_down -= GetFrameTime();
    }

    // move the player towards the target by their speed
    // if the player hasn't reached the goal
    if (!reached_goal && cool_down < 0.0f){
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
                // determine when goal has been reached by proximity to other units that have reached the similar goal
                if (circle_circle_intersection(vec3_to_vec2(position), vec3_to_vec2(i->position), 0.3f, 0.3f)){
                    blockage = true;
                    avoidance_force = (Vector3){ position.x - i->position.x, 0.0f, position.z - i->position.z };
                    avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                    position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                    if (i->reached_goal && Vector3DistanceSqr(goal, i->goal) < 1.0f){
                        reached_goal = true;
                    }   
                } else {
                    // collisiotn avoidance
                    // check if any units are in the are the sprite is travelling towards
                    if (point_circle_intersection(vec3_to_vec2(ahead), vec3_to_vec2(i->position), 0.5f )){
                         if (Vector3DistanceSqr(goal, i->goal) > 1.0f){
                            blockage = true;
                            // appply an avoidance force to the player to avoid unit
                            avoidance_force = (Vector3){ ahead.x - i->position.x, 0.0f, ahead.z - i->position.z };
                            avoidance_force = Vector3Scale(Vector3Normalize(avoidance_force), MAX_AVOIDANCE_FORCE);
                            position = move_to_target(position, Vector3Add(ahead, avoidance_force), speed);
                         }
                    }
                }
            }
            // if no collision avoidance, move towards target
            if (!blockage){
                position = move_to_target(position, target, speed);
            }
        }    
    }
}

void Archer::update_target(Vector3 new_target){
    // update the goa
    goal = new_target;
    reached_goal = false;
    // get the next node in the route to the goal
    Vector2 next_target = env->gen_route(vec3_to_vec2(position), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Archer::next_node(){
    // get the next node in the route to the goal
    Vector2 next_target = env->gen_route(vec3_to_vec2(target), vec3_to_vec2(goal));
    target = vec2_to_vec3_ground(next_target);
}

void Archer::draw(){
    // draw the correct teams archer model
    if (team == Teams::red_team){
        DrawModel(archer_model, position, 0.3f, RED);
    } else if (team == Teams::blue_team){
        DrawModel(archer_model, position, 0.3f, BLUE);
    }

    // draw the halo if it is a 'selected' unit
    if (selected){
        DrawModel(halo_model, position, 0.3f, WHITE);
    }

}

void Archer::toggle_attack(){
    if (attack_mode == false){
        attack_mode = true;
    }
    else{
        attack_mode = false;
    }
}

Archer::~Archer(){
    UnloadShader(shader);
    UnloadModel(archer_model);
    UnloadModel(halo_model);
}
