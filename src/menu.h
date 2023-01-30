#ifndef __MENU__
#define __MENU__

#include "raylib.h"
#include "raymath.h"

#define SERVER 0
#define CLIENT 1
#define LOCAL 2
#define QUIT 3

class Button
{
    private:
        Vector2 pos;
        float width, height;
        char* text;
        Color chosen_colour;
        Color hovered_colour;
        Color colour;
        int fontsize;
    public:
        Button(Vector2 pos, float width, float height, Color col, char* text, int fontsize);
        bool on_button(Vector2 mouse);
        void update();
        void draw();
};

int run_menu();

#endif