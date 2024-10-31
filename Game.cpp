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

	if (state != Running) { // Added a method to stop updating game logic when not running.
		if (GetAsyncKeyState('R') & 0x1) {
			Reset();
			state = Running;
		}
		return true;
	}

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
	static GameState lastState = GameState::Running; // This tracks the last state to detect changes
	Console::Lock(true);
	Console::Clear();
	
	if (state == Running) {
		paddle.Draw();
		ball.Draw();

		// TODO #3 - Update render to render all bricks
		for (const Box& brick : bricks) {
			brick.Draw();
		}
	}
	else if (state == Won) {
		int x = Console::WindowWidth() / 2 - 15;
		int y = Console::WindowHeight() / 2;
		Console::WordWrap(x, y, 30, "You win! Press 'R' to play again.");
	}
	else if (state == Lost) {
		int x = Console::WindowWidth() / 2 - 15;
		int y = Console::WindowHeight() / 2;
		Console::WordWrap(x, y, 30, "You lose. Press 'R' to play again.");
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (auto it = bricks.begin(); it != bricks.end();) {
		if (it->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity)) {
			it->hitCount++; // This increments the hit count.
			ball.y_velocity *= -1;  // This reverses the ball's direction.

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			switch (it->hitCount) {
			case 1:
				it->color = ConsoleColor::DarkYellow;  // First hit color
				break;
			case 2:
				it->color = ConsoleColor::DarkRed;  // Second hit color
				break;
			case 3:
				it = bricks.erase(it);  // Remove the brick if hit 3 times
				continue;  // Skip the increment step since the iterator is now at the next element
			}
			++it;  // Move to the next brick if not removed
		}
		else {
			++it;  // Move to the next brick if no collision occurred
		}
	}

		// TODO #6 - If no bricks remain, pause ball and display victory text with R to reset
	if (bricks.empty()) {
		state = GameState::Won; // This changes the game state to won.
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
		if (ball.y_position + ball.y_velocity >= Console::WindowHeight()) {
			state = GameState::Lost; // This changes the game state to lost.
			ball.moving = false;
			Console::Clear();
			int x = Console::WindowWidth() / 2 - 15;
			int y = Console::WindowHeight() / 2;
			Console::WordWrap(x, y, 30, "You lose. Press 'R' to play again.");
		}
	}

