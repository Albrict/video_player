#include "label.hpp"
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(1024, 768, "VideoPlayer");
    ShGUI::Button button({512, 384, 200, 600}, BLUE, "Test");
    ShGUI::Label label({100, 100, 100, 300}, RED, "FUCK");
     
    button.setCallback(exitButtonCallback, nullptr);
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        button.proccessEvents();
        button.update();
        BeginDrawing();
            ClearBackground(BLACK);
            button.draw();
            label.draw();
        EndDrawing();
    }
    return 0;
}
