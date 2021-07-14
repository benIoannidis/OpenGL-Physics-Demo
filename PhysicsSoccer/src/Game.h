#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Input.h"
#include <chrono>

class PhysicsScene;
class Box;
class Plane;
class Sphere;
enum GameState
{
	RUNNING,
	P1SCORED,
	P2SCORED,
	NOTIME,
	P1WIN,
	P2WIN,
	DRAW,
	MENU
};
class Game
{
public:
	Game(glm::vec2, glm::vec2, glm::vec2, float, float);

	GameState CheckState(); //return current gamestate (mostly for UI purposes)

	void GameStart(); //set current state to RUNNING

	void Update(float deltaTime); //check state and perfom time checks and update physics scene if RUNNING, otherwise act as if game is in a paused state 
	void Draw(); //draw centre lines and goal rects, and begin physics scene drawing as well

	void GameEnd(); //check state of score

	void GameRestart(); //reset state, scores, time, and all positions

	void CheckInput(); //check for player inputs

	void CheckScored(); //check if the ball is completely inside the goal areas

	glm::vec2 GetP1Position(); //return p1 pos
	glm::vec2 GetP2Position(); //return p2 pos

	void ResetPositions(); //remove all actors and reinstantiate them in their original positions

	float GetTime(); //return current time for UI purposes

	int GetP1Score(); //return p1 score
	int GetP2Score(); //return p2 score

protected:
	PhysicsScene* m_physicsScene;

	GameState m_state;

	Box* playerOne;
	Box* playerTwo;
	Plane* hBounds[2];
	Plane* vBounds[2];
	Sphere* ball;

	float playerRotationForce = 10.f;

	float playerMovementForce = 5.f;

	glm::vec2 ballSpawn;

	glm::vec2 playerOneSpawn, playerTwoSpawn;

	glm::vec2 leftGoal, rightGoal;
	glm::vec4 leftGoalColour, rightGoalColour;


	float goalHeight, goalWidth;

	float time;
	std::chrono::steady_clock::time_point gameTime;
	bool gameTimerStarted = false;

	int p1Score, p2Score;

	aie::Input* input;

	std::chrono::steady_clock::time_point chronoTimer;
	bool timerStarted = false;
};

