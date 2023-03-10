#include "menu.h"

Button::Button(Vector2 pos, float width, float height, Color col, char* text, int fontsize){
    this->pos = pos;
    this->width = width;
    this->height = height;
    this->text = text;
    this->chosen_colour = col;
    this->colour = col;
    this->fontsize = fontsize;
    // create a second colour based on the original that is for when the mouse hovers over the button
    this->hovered_colour = (Color){ static_cast<unsigned char>(chosen_colour.r - 10), static_cast<unsigned char>(chosen_colour.g - 10), static_cast<unsigned char>(chosen_colour.b - 10), chosen_colour.a };
}

void Button::update(){
    // update the colour of the button depending on whether the mouse is hovering over it
    if (on_button(GetMousePosition())){
        colour = hovered_colour;
    } else{
        colour = chosen_colour;
    }
}

bool Button::on_button(Vector2 mouse){
    // check if mouse is hovering over button
    if (mouse.x >= pos.x && mouse.x <= pos.x + width && mouse.y >= pos.y && mouse.y <= pos.y + height){
        return true;
    }
    return false;
}

void Button::draw(){
    // draw the rectangle and text of the button
    DrawRectangle(pos.x, pos.y, width, height, colour);
    DrawText(text, pos.x + 10, pos.y + 10, fontsize, BLACK);
}

int run_menu(){
    // generate relevant buttons for server, client, local and quit
    Button server_button = Button((Vector2){250.0f*GetScreenWidth()/1600, 250.0f*GetScreenHeight()/1000}, 500*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "Server", 50*GetScreenWidth()/1600);
    Button client_button = Button((Vector2){800.0f*GetScreenWidth()/1600, 250.0f*GetScreenHeight()/1000}, 500*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "Client", 50*GetScreenWidth()/1600);
    Button local_button = Button((Vector2){700.0f*GetScreenWidth()/1600, 400.0f*GetScreenHeight()/1000}, 400*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, ORANGE, "Local", 50*GetScreenWidth()/1600);
    Button quit_button = Button((Vector2){390.0f*GetScreenWidth()/1600, 400.0f*GetScreenHeight()/1000}, 200*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, GREEN, "QUIT", 50*GetScreenWidth()/1600);

    bool running = true;

    SetTargetFPS(60);  

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // if the mouse is released test if it is hitting any buttons
        if (IsMouseButtonReleased(0)){
            if(server_button.on_button(GetMousePosition())){
                return SERVER;
            }
            else if(client_button.on_button(GetMousePosition())){
                return CLIENT;
            }
            else if(local_button.on_button(GetMousePosition())){
                return LOCAL;
            }
            else if(quit_button.on_button(GetMousePosition())){
                return QUIT;
            }
        }

        // update all buttons
        server_button.update();
        client_button.update();
        local_button.update();
        quit_button.update();


        // run the draw loop
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // draw title
            DrawText("Territorio", (GetRenderWidth()/2) - 290*GetScreenWidth()/1600, 100*GetScreenHeight()/1000, 100*GetScreenWidth()/1600, BLACK);

            // draw buttons
            server_button.draw();
            client_button.draw();
            local_button.draw();
            quit_button.draw();
        EndDrawing();
    }
    // if window closes, quit program
    return QUIT;
}