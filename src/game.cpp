#include "game.h"

Game::Game(){
    std::cout << "new game" << std::endl;

}

void Game::initialise_game(){

    env = new Environment((char*)"assets/scene/main_level.txt");
    sprites.push_back(new Archer((Vector3){ 12.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Archer((Vector3){ 14.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 13.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 15.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 16.0f, 0.0f, 10.0f }, Teams::blue_team, env));

    sprites.push_back(new Archer((Vector3){ 5.0f, 0.0f, 10.0f }, Teams::red_team, env));
    sprites.push_back(new Archer((Vector3){ 5.0f, 0.0f, 11.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 12.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 13.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 14.0f }, Teams::red_team, env));


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // Define the camera to look into 3D world
    camera.position = (Vector3){ 20.0f, 10.0f, 20.0f }; // Camera position
    camera.target = (Vector3){ 10.0f, 0.0f, 10.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 20.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC; 
}

void Game::event_update_draw(){

    // Update
    for (Sprite* i : sprites) {
        i->update(sprites);
    }

    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
            env->draw();

            for (Sprite* i : sprites) {
                i->draw();
            }

        EndMode3D();

    EndDrawing();
}

void Game::events(Teams team){
    // Events
    Vector3 ground_intersect;
    Vector2 corner1;
    Vector2 corner2;
    Vector2 corner3;
    Vector2 corner4;

    Ray ray = { 0 };

    
    ray = GetMouseRay(GetMousePosition(), camera);
    if (IsMouseButtonPressed(1)){
        ground_intersect = ray_ground_intersection(ray);
        if(env->valid_target((Vector2){ ground_intersect.x, ground_intersect.z })){
            ground_intersect.y += 0.5f;
            for (Sprite* i : sprites) {
                // if (i->team == team){
                    if (i->Type == Sprite_Type::worker_unit){
                        auto TypedI = (Worker*) i;
                        if (TypedI->selected){
                            TypedI->update_target(ground_intersect);
                        }
                    }
                    else if (i->Type == Sprite_Type::archer_unit){
                        auto TypedI = (Archer*) i;
                        if (TypedI->selected){
                            TypedI->update_target(ground_intersect);
                        }
                    } 
                // }
            }
        }
    }

    if (IsMouseButtonPressed(0)){
        corner1 = GetMousePosition();
        
    }
    if (IsMouseButtonReleased(0)){
        corner2 = GetMousePosition();
        corner3 = (Vector2){ corner1.x, corner2.y };
        corner4 = (Vector2){ corner1.y, corner2.x };

        for (Sprite* i : sprites) {
            // if(i->team == team)
                i->is_selected(ray_ground_intersection(GetMouseRay(corner1, camera)),
                    ray_ground_intersection(GetMouseRay(corner2, camera)), 
                    ray_ground_intersection(GetMouseRay(corner3, camera)), 
                    ray_ground_intersection(GetMouseRay(corner4, camera)));
            // }
        }
    }

    if (IsKeyPressed(KEY_W)){
        for (Sprite* i : sprites) {
            // if(i->team == team){
                if (i->Type == Sprite_Type::worker_unit){
                    auto typedI = (Worker*) i;
                    if (typedI->selected){
                        typedI->new_mine_area();
                    }
                }
            // }
        }
    }
}

Game::~Game(){
    delete env;
    
    for (Sprite* i : sprites) {
        
        switch (i->Type)
        {
        case Sprite_Type::worker_unit:
            delete (Worker*) i;
            break;
        case Sprite_Type::archer_unit:
            delete (Archer*) i;
            break;
        
        default:
            break;
        }
    }
}

void C_Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    init_network();
    while (!WindowShouldClose()){ // Detect window close button or ESC key
        network();
        events(Teams::red_team);
        event_update_draw();
    }

    CloseWindow();
}

void C_Game::init_network(){
    client = Client(12531);
}

void C_Game::network(){
    std::vector<Byte> bytes = client.recieve();
    for (Byte i: bytes){
        i.print();
        std::cout << ", ";
    }
    Deserialise unpack(bytes);
    for(Sprite* i: sprites){
        if(unpack.get_next_type() != Data_Types::finished){
            i->position.x = unpack.get_real();
            i->position.z = unpack.get_real();
        }
    }
}


void S_Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    init_network();
    while (!WindowShouldClose()){ // Detect window close button or ESC key
        network();
        events(Teams::blue_team);
        event_update_draw();
    }

    CloseWindow();
}

void S_Game::init_network(){
    server = Server(12531);
}

void S_Game::network(){
    Serialise pack;
    for(Sprite* i: sprites){
        pack = pack << i->position.x << i->position.z;
    }
    server.write(pack.get_bytes());
}
