#include <array>

#include "raylib.h"
#include "raymath.h"

#include "Map.hpp"

int main(int argc, char *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, argv[0]);
    SetTargetFPS(144);

    Camera2D camera{
        {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
        {0.0f,                    0.0f                    },
        0.0f, 1.0f
    };
    Map map;

    while (!WindowShouldClose())
    {
        float movementSpeed = 500.0f * GetFrameTime() / camera.zoom;
        if (IsKeyDown(KEY_W))
        {
            camera.target.y -= movementSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            camera.target.y += movementSpeed;
        }
        if (IsKeyDown(KEY_A))
        {
            camera.target.x -= movementSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            camera.target.x += movementSpeed;
        }
        camera.zoom = Clamp(camera.zoom + GetMouseWheelMove() * 0.1f, 0.25f, 4.0f);
        camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        BeginDrawing();
        {
            ClearBackground(DARKGRAY);

            BeginMode2D(camera);
            {
                map.Draw();
            }
            EndMode2D();

            Vector2 mousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                map.OnMouseButtonLeftDown(mousePosition);
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                map.OnMouseButtonLeftPressed();
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                map.OnMouseButtonLeftReleased();
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                map.OnMouseButtonRightPressed(mousePosition);
            }
        }
        EndDrawing();
    }

    return 0;
}
