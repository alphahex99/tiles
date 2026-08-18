#include "raylib.h"

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, argv[0]);
    SetTargetFPS(144);

    while (!WindowShouldClose())
    {
    }

    return 0;
}
