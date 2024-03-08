#include "raylib.h"
#include "button.hpp"
#include <cstdlib>

void exitButtonCallback(void *data)
{
    CloseWindow();
    std::exit(0);
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1024, 768, "VideoPlayer");
    ShGUI::Button button({512, 384, 100, 100}, BLUE);
    button.setCallback(exitButtonCallback, nullptr);
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        button.proccessEvents();
        button.update();
        BeginDrawing();
            ClearBackground(BLACK);
            button.draw();
        EndDrawing();
    }
    return 0;
}
