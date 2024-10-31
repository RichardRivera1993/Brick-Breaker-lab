#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear(); // This will clear existing bricks if their is any.
	int num_bricks = 5;
	int brick_spacing = WINDOW_WIDTH / (num_bricks + 1); // This calculates spacing to center bricks evenly.
	for (int i = 0; i < num_bricks; i++) {
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = brick_spacing * (i+1) - brick.width / 2; // This positions the bricks evenly.
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;
		bricks.push_back(brick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering should be done between the locks in this function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (const Box& brick : bricks) {
		brick.Draw();
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (auto it = bricks.begin(); it != bricks.end();) {
		if (it->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity)) {
			it->color = ConsoleColor(it->color - 1);  // Change color on hit
			ball.y_velocity *= -1;  // Reverse the ball's direction

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (it->color == ConsoleColor::Black) {
				it = bricks.erase(it);  // Remove brick and move to the next
			}
			else {
				++it;  // Only increment if not erased
			}
		}
		else {
			++it;
		}
	}

		// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	if (bricks.empty()) {
		ball.moving = false;
		Console::Clear();
		int x = Console::WindowWidth() / 2 - 15;
		int y = Console::WindowHeight() / 2;
		Console::WordWrap(x, y, 30, "You win! Press 'R' to play again.");
	}

		if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
		{
			ball.y_velocity *= -1;
		}

		// TODO #7 - If ball touches bottom of window, pause ball and display defeat text with R to reset
	}
}
