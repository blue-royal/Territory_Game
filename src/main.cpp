#include "raylib.h"
#include "environment.h"
#include "worker.h"

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(800, 500, "Territory");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Environment env = Environment((char*)"assets/scene/main_level.txt");
    Worker work = Worker(BLUE);

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 20.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_ORTHOGRAPHIC; 


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);

                work.update();

                env.draw();
                work.draw();
                DrawGrid(10, 1.0f);         // Draw a grid

            EndMode3D();



        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}