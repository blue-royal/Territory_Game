#include "menu.h"

Button::Button(Vector2 pos, float width, float height, Color col, char* text, int fontsize){
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->text = text;
    this->chosen_colour = col;
    this->colour = col;
    this->fontsize = fontsize;

    this->hovered_colour = (Color){ static_cast<unsigned char>(chosen_colour.r - 10), static_cast<unsigned char>(chosen_colour.g - 10), static_cast<unsigned char>(chosen_colour.b - 10), chosen_colour.a };
}

void Button::update(){
    if (on_button(GetMousePosition())){
        colour = hovered_colour;
    } else{
        colour = chosen_colour;
    }
}

bool Button::on_button(Vector2 mouse){
    if (mouse.x >= pos.x && mouse.x <= pos.x + width && mouse.y >= pos.y && mouse.y <= pos.y + height){
        return true;
    }
    return false;
}

void Button::draw(){
    DrawRectangle(pos.x, pos.y, width, height, colour);
    DrawText(text, pos.x + 10, pos.y + 10, fontsize, BLACK);
}

int run_menu(){
    Button server_button = Button((Vector2){250.0f*GetScreenWidth()/1600, 250.0f*GetScreenHeight()/1000}, 500*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "Server", 50*GetScreenWidth()/1600);
    Button client_button = Button((Vector2){800.0f*GetScreenWidth()/1600, 250.0f*GetScreenHeight()/1000}, 500*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "Client", 50*GetScreenWidth()/1600);
    Button quit_button = Button((Vector2){390.0f*GetScreenWidth()/1600, 400.0f*GetScreenHeight()/1000}, 200*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "QUIT", 50*GetScreenWidth()/1600);

    bool running = true;

    SetTargetFPS(60);  

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // run the event loop
        if (IsMouseButtonReleased(0)){
            if(server_button.on_button(GetMousePosition())){
                EndDrawing();
                return SERVER;
            }
            else if(client_button.on_button(GetMousePosition())){
                EndDrawing();
                return CLIENT;
            }
            else if(quit_button.on_button(GetMousePosition())){
                EndDrawing();
                return QUIT;
            }
        }

        server_button.update();
        client_button.update();
        quit_button.update();


        // run the draw loop
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Territorio", (GetRenderWidth()/2) - 290*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, 100*GetScreenWidth()/1600, BLACK);

            server_button.draw();
            client_button.draw();
            quit_button.draw();
        EndDrawing();
    }
    return QUIT;
}