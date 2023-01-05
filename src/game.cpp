#include "game.h"

Game::Game(){
    std::cout << "new game" << std::endl;
}

void Game::initialise_game(){

    Server network = Server();
    Client client = Client();

    env = new Environment((char*)"assets/scene/main_level.txt");
    sprites.push_back(new Worker((Vector3){ 12.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 14.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 13.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 15.0f, 0.0f, 10.0f }, Teams::blue_team, env));
    sprites.push_back(new Worker((Vector3){ 16.0f, 0.0f, 10.0f }, Teams::blue_team, env));

    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 10.0f }, Teams::red_team, env));
    sprites.push_back(new Worker((Vector3){ 5.0f, 0.0f, 11.0f }, Teams::red_team, env));
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

void Game::game_loop(){
    Vector3 ground_intersect;
    Vector3 corner1;
    Vector3 corner2;
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Events
        Ray ray = { 0 };
        ray = GetMouseRay(GetMousePosition(), camera);
        if (IsMouseButtonPressed(1)){
            ground_intersect = ray_ground_intersection(ray);
            if(env->valid_target((Vector2){ ground_intersect.x, ground_intersect.z })){
                ground_intersect.y += 0.5f;
                for (Sprite* i : sprites) {
                    if (i->Type == Sprite_Type::worker_unit){
                        auto TypedI = (Worker*) i;
                        if (TypedI->selected){
                            TypedI->update_target(ground_intersect);
                        }
                    }
                    
                }
            }
        }

        if (IsMouseButtonPressed(0)){
            corner1 = ray_ground_intersection(GetMouseRay(GetMousePosition(), camera));
            
        }
        if (IsMouseButtonReleased(0)){
            corner2 = ray_ground_intersection(GetMouseRay(GetMousePosition(), camera));
            for (Sprite* i : sprites) {
                i->is_selected(corner1, corner2);
            }
        }

        if (IsKeyPressed(KEY_W)){
            for (Sprite* i : sprites) {
                if (i->Type == Sprite_Type::worker_unit){
                    auto typedI = (Worker*) i;
                    if (typedI->selected){
                        typedI->new_mine_area();
                    }
                }
            }
        }

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
}

void Game::run_game(){
    std::cout << "running" << std::endl;
    initialise_game();
    game_loop();

    CloseWindow();
}

Game::~Game(){
    delete env;
    
    for (std::vector<Sprite*>::iterator i = sprites.begin(); i < sprites.end(); i++) {
        delete *i;
    }
}
