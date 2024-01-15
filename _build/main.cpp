#include <iostream>
#include "raylib.h"
#include <deque>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };


int cellSize = 30;
int cellCount = 25;

class Snake {
public: 
    deque<Vector2>  body = { Vector2{6, 9} , Vector2{5, 9} , Vector2{4, 9} };

    void Draw()
    {
        for (int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;

            Rectangle segment = Rectangle{
                x*cellSize , y*cellSize, (float)cellSize, (float)cellSize
            };

            DrawRectangleRounded(segment,0.5, 10 ,darkGreen);
        }
    }

};


class Food {

public:
    Vector2 position;
    Texture2D texture;

    Food() {
        position = GenerateRandomPos();
        Image image = LoadImage("resources/food.png");
        ImageResize(&image, 28, 28);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);

        return Vector2{ x, y };
    }

};


int main() {
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Food food = Food();
    Snake snake = Snake();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(green);
        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
