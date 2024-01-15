#include <iostream>
#include <deque>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };


int cellSize = 30;
int cellCount = 25;


float lastUpdateTime = 0;
bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;

    }
    return false;
}

class Snake {
public: 
    deque<Vector2>  body = { Vector2{6, 9} , Vector2{5, 9} , Vector2{4, 9} };
    Vector2 direction = { 1 , 0 };

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

    void Update() {
        
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));


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

class Game {
public:
    Snake snake = Snake();
    Food food = Food();

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        snake.Update();
    }
};


int main() {
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();

        game.Draw();

        ClearBackground(green);
  

        if (eventTriggered(0.2)) {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = { 0 , -1};
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = { 0 , 1 };
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = { 1 , 0 };
        }
        if (IsKeyPressed(KEY_LEFT)  && game.snake.direction.x != 1) {
            game.snake.direction = { -1 , 0 };
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
