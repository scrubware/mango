//
// Created by jarboola on 2/2/26.
//

module;

#include <functional>
#include <raylib.h>

export module Mango;

export import Mango.Math;
export import Mango.Color;
export import Mango.Cam;
export import Mango.Input;
export import Mango.Schedule;
export import Mango.Screen;
export import Mango.Core;

export namespace Mango {

    Col color = White;

    int window_width = 320 * 6;
    int window_height = 180 * 6;

    void Initialize(const char* window_name) {
        InitWindow(window_width, window_height, window_name);
    }

    void Initialize(const char* window_name, const int window_width, const int window_height) {
        InitWindow(window_width, window_height, window_name);
        Mango::window_width = window_width;
        Mango::window_height = window_height;
    }

    void MainLoop(const std::function<void()>& main) {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            main();
            EndDrawing();
        }

        CloseWindow();
    }

    Col Color() {
        return color;
    }

    void DrawCircle(const float x, const float y, const float radius, const Col color = Color()) {
        DrawCircle(x,y,radius,color.color);
    }
    void DrawCircle(const Vec2 origin, const float radius, const Col color = Color()) {
        DrawCircle(origin.x,origin.y,radius,color);
    }
}


