#include "raylib.h"
#include "window.hpp"

int main()
{
    VideoPlayer::Window win(640, 480, "VideoPlayer");
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Hello, world!", 320, 240, 28, WHITE);
        EndDrawing();
    }
    return 0;
}
