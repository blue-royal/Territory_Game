#include "game.h"

Game::Game(){
    std::cout << "new game" << std::endl;
}

void Game::initialise_game(){
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
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Events
        Ray ray = { 0 };
        ray = GetMouseRay(GetMousePosition(), camera);
        if (IsMouseButtonPressed(1)){
            ground_intersect = ray_ground_intersection(ray);
            if(env.valid_target((Vector2){ ground_intersect.x, ground_intersect.z })){
                ground_intersect.y += 0.5f;
                Vector2 start = (Vector2) {work.position.x, work.position.z};
                Vector2 end = (Vector2) {ground_intersect.x, ground_intersect.z};
                Vector2 target = env.gen_route(start, end);
                work.update_target((Vector3){ target.x, ground_intersect.y , target.y });
            }
        }
        // Update
        work.update();
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
                env.draw();
                work.draw();

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
