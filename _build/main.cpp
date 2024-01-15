#include <iostream>
#include <deque>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };


int cellSize = 30;
int cellCount = 25;
float offset = 75;



float lastUpdateTime = 0;
bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;

	}
	return false;
}



bool isPresentInBody(Vector2 position, deque<Vector2> body) {

	for (unsigned int i = 0; i < body.size(); i++) {
		if (Vector2Equals(position, body[i]))
			return true;
	}

	return false;
}


class Snake {
public:
	deque<Vector2>  body = { Vector2{6, 9} , Vector2{5, 9} , Vector2{4, 9} };
	Vector2 direction = { 1 , 0 };
	bool addSegment = false;

	void Draw()
	{
		for (int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;

			Rectangle segment = Rectangle{
				x * cellSize + offset , y * cellSize + offset, (float)cellSize, (float)cellSize
			};

			DrawRectangleRounded(segment, 0.5, 10, darkGreen);
		}

	}

	void Update() {


		body.push_front(Vector2Add(body[0], direction));
		if (addSegment) {
			addSegment = false;
		}
		else {
			body.pop_back();
		}


	}

	void Reset() {
		body = { Vector2{6, 9} , Vector2{5, 9} , Vector2{4, 9} };
		direction = { 1,0 };
	}

};

class Food {

public:
	Vector2 position;
	Texture2D texture;

	Food(deque<Vector2> snakeBody) {
		position = GenerateRandomPos(snakeBody);
		Image image = LoadImage("resources/food.png");
		ImageResize(&image, 28, 28);
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, position.x * cellSize + offset, position.y * cellSize + offset, WHITE);
	}

	Vector2 GenerateRandomCell() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);

		return Vector2{ x,y };
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {

		Vector2 position = GenerateRandomCell();

		while (isPresentInBody(position, snakeBody))
		{
			position = GenerateRandomCell();
		}

		return position;
	}

};

class Game {
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;
	Sound eatSound;
	Sound wallSound;

	Game() {
		InitAudioDevice();
		eatSound = LoadSound("resources/eat.mp3");
		wallSound = LoadSound("resources/wall.mp3");
	}

	~Game() {
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}

	void Draw() {
		food.Draw();
		snake.Draw();
	}

	void Update() {
		if (running) {
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}

	void CheckCollisionWithFood() {
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
			PlaySound(eatSound);
		}
	}

	void CheckCollisionWithEdges() {
		if (snake.body[0].x == cellCount || snake.body[0].x == -1) {
			GameOver();
		}

		if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
			GameOver();
		}
	}

	void CheckCollisionWithTail() {
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (isPresentInBody(snake.body[0], headlessBody)) {
			GameOver();
		}

	}

	void GameOver() {
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		score = 0;
		PlaySound(wallSound);
	}
};


int main() {
	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Game game = Game();

	while (!WindowShouldClose()) {
		BeginDrawing();

		game.Draw();

		if (eventTriggered(0.2)) {
			game.Update();
		}

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
			game.snake.direction = { 0 , -1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
			game.snake.direction = { 0 , 1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
			game.snake.direction = { 1 , 0 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
			game.snake.direction = { -1 , 0 };
			game.running = true;
		}


		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{ offset - 5, offset - 5,(float) cellSize * cellCount + 10,(float) cellSize * cellCount + 10 }, 5, darkGreen);
		DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
		DrawText(TextFormat("%i" , game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
