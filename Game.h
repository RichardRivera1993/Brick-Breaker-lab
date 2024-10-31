#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	enum GameState { Running, Won, Lost } state = Running;// created a gamestate variable.
	Ball ball;
	Box paddle;

	// TODO #1 - Instead of storing 1 brick, store a vector of bricks (by value)
	std::vector<Box>bricks;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};