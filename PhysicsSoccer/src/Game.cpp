#include "Game.h"
#include "PhysicsScene.h"
#include "Box.h"
#include "Plane.h"
#include "Sphere.h"
#include "Gizmos.h"



Game::Game(glm::vec2 playerOnePos, glm::vec2 playerTwoPos, glm::vec2 ballPos, float boundYFromOrigin, float boundXFromOrigin)
{
	playerOneSpawn = playerOnePos;
	playerTwoSpawn = playerTwoPos;
	playerOne = new Box(playerOneSpawn, glm::vec2(0), 4.f, 10.f, 10.f, 0.1f, glm::vec4(0, 0, 1, 1));
	playerTwo = new Box(playerTwoSpawn, glm::vec2(0), 4.f, 10.f, 10.f, 0.3f, glm::vec4(1, 0, 0, 1));

	ballSpawn = ballPos;
	ball = new Sphere(ballSpawn, glm::vec2(0), 1.f, 4.f, 1.f, glm::vec4(1, 0.7f, 0, 1));

	hBounds[0] = new Plane(glm::vec2(0, -1), -boundYFromOrigin);
	hBounds[1] = new Plane(glm::vec2(0, 1), -boundYFromOrigin);

	vBounds[0] = new Plane(glm::vec2(1, 0), -boundXFromOrigin);
	vBounds[1] = new Plane(glm::vec2(-1, 0), -boundXFromOrigin);

	goalHeight = 20.f;
	goalWidth = 10.f;

	leftGoal = glm::vec2(-90, 0);
	rightGoal = glm::vec2(90, 0);
	leftGoalColour = glm::vec4(0, 0, 1, 1);
	rightGoalColour = glm::vec4(1, 0, 0, 1);

	p1Score = 0;
	p2Score = 0;

	time = 60.f;

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0));
	m_physicsScene->SetTimeStep(0.0001f);
	m_physicsScene->AddActor(playerOne);
	m_physicsScene->AddActor(playerTwo);
	m_physicsScene->AddActor(ball);

	for (auto bound : hBounds)
	{
		m_physicsScene->AddActor(bound);
	}
	for (auto bound : vBounds)
	{
		m_physicsScene->AddActor(bound);
	}

	input = aie::Input::getInstance();
	m_state = MENU;
}

GameState Game::CheckState()
{
	return m_state;
}

void Game::GameStart()
{
	m_state = RUNNING;
}

void Game::CheckInput()
{
#pragma region p1 controls 
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		//move p1 up
		playerOne->ApplyForce(glm::vec2(0, playerMovementForce), -playerOne->GetLocalY());
	}
	else if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		//move p1 down
		playerOne->ApplyForce(glm::vec2(0, -playerMovementForce), playerOne->GetLocalY() + glm::vec2(0, 5));
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		//move p1 left
		playerOne->ApplyForce(glm::vec2(-playerMovementForce, 0), playerOne->GetLocalX());
		
	}
	else if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		//move p1 right
		playerOne->ApplyForce(glm::vec2(playerMovementForce, 0), -playerOne->GetLocalX());
	}
	if (input->isKeyDown(aie::INPUT_KEY_Q))
	{
		playerOne->NullAngularVelocity();
		float or = playerOne->GetOrientation();
		or += 0.1f;
		playerOne->SetOrientation(or);
	}
	else if (input->isKeyDown(aie::INPUT_KEY_E))
	{
		playerOne->NullAngularVelocity();
		float or = playerOne->GetOrientation();
		or -= 0.1f;
		playerOne->SetOrientation(or);
	}
#pragma endregion

#pragma region p2 controls
	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		//move p2 up
		playerTwo->ApplyForce(glm::vec2(0, playerMovementForce), -playerTwo->GetLocalY());
	}
	else if (input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		//move p2 down
		playerTwo->ApplyForce(glm::vec2(0, -playerMovementForce), playerTwo->GetLocalY());
	}
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		//move p2 left
		playerTwo->ApplyForce(glm::vec2(-playerMovementForce, 0), playerTwo->GetLocalX());
	}
	else if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		//move p2 right
		playerTwo->ApplyForce(glm::vec2(playerMovementForce, 0), -playerTwo->GetLocalX());
	}
	if (input->isKeyDown(aie::INPUT_KEY_COMMA))
	{
		playerTwo->NullAngularVelocity();
		float or = playerTwo->GetOrientation();
		or += 0.1f;
		playerTwo->SetOrientation(or);
	}
	else if (input->isKeyDown(aie::INPUT_KEY_PERIOD))
	{
		playerTwo->NullAngularVelocity();
		float or = playerTwo->GetOrientation();
		or -= 0.1f;
		playerTwo->SetOrientation(or);
	}
#pragma endregion
}

void Game::CheckScored()
{
	float ballR = ball->GetRadius();
	float ballX = ball->GetPosition().x;
	float ballY = ball->GetPosition().y;

	if (ballX + ballR <= leftGoal.x + goalWidth &&
		(ballY + ballR <= leftGoal.y + goalHeight && ballY - ballR >= leftGoal.y - goalHeight))
	{
		m_state = P2SCORED;
		if (p2Score < 2)
		{
			p2Score++;
		}
		else
		{
			GameEnd();
		}
	}
	else if (ballX - ballR >= rightGoal.x - goalWidth && (ballY + ballR <= rightGoal.y + goalHeight && ballY - ballR >= rightGoal.y - goalHeight))
	{
		m_state = P1SCORED;

		if (p1Score < 2)
		{
			p1Score++;
		}
		else
		{
			GameEnd();
		}
	}
}

void Game::Update(float deltaTime)
{
	if (m_state == RUNNING)
	{
		if (time > 0)
		{
			if (!gameTimerStarted)
			{
				gameTimerStarted = true;
				gameTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
			}
			else if (gameTime < std::chrono::steady_clock::now())
			{
				gameTimerStarted = false;
				time--;
			}
			CheckInput();
			CheckScored();
		}
		else
		{
			GameEnd();
		}

	}
	else
	{
		ball->NullVelocity();
		
		if ((m_state == P1SCORED || m_state == P2SCORED) && !timerStarted)
		{
			chronoTimer = std::chrono::steady_clock::now() + std::chrono::seconds(3);
			timerStarted = true;
		}
		else if (m_state == P1SCORED || m_state == P2SCORED)
		{
			if (chronoTimer < std::chrono::steady_clock::now())
			{
				ResetPositions();
				m_state = RUNNING;
				timerStarted = false;
			}
		}
		else if ((m_state == P1WIN || m_state == P2WIN || m_state == DRAW || m_state == NOTIME) && !timerStarted)
		{
			chronoTimer = std::chrono::steady_clock::now() + std::chrono::seconds(5);
			timerStarted = true;
		}
		else if (m_state == P1WIN || m_state == P2WIN || m_state == DRAW || m_state == NOTIME)
		{
			if (chronoTimer < std::chrono::steady_clock::now())
			{
				GameRestart();
				timerStarted = false;
			}
		}
	}
	m_physicsScene->Update(deltaTime);

	
}

void Game::Draw()
{
	//draw centre line
	glm::vec4 m_colour = glm::vec4(0.75f, 1, 0.75f, 1);
	glm::vec4 groundColour = glm::vec4(0.2f, 0.5f, 0.2f, 1.f);
	//aie::Gizmos::add2DTri(start, end, start - m_normal * 2.0f, m_colour, m_colour, m_colour);
	aie::Gizmos::add2DTri(glm::vec2(-1, 500), glm::vec2(1, 500), glm::vec2(-1, -500), m_colour);
	aie::Gizmos::add2DTri(glm::vec2(-1, -500), glm::vec2(1, -500), glm::vec2(1, 500), m_colour);
	aie::Gizmos::add2DCircle(glm::vec2(0), 10, 500, m_colour);
	aie::Gizmos::add2DCircle(glm::vec2(0), 9, 500, groundColour);


	//draw goal boxes
	aie::Gizmos::add2DAABB(glm::vec2(leftGoal.x + goalWidth * 0.5f, leftGoal.y), glm::vec2(goalWidth * 0.5f, goalHeight), glm::vec4(0.f, 0.f, 1.f, 1.f));
	aie::Gizmos::add2DAABB(glm::vec2(rightGoal.x - goalWidth * 0.5f, rightGoal.y), glm::vec2(goalWidth * 0.5f, goalHeight), glm::vec4(1.f, 0.f, 0.f, 1.f));
	//aie::Gizmos::add2DTri(end, end - m_normal * 2.0f, start - m_normal * 2.0f, m_colour, m_colour, m_colour/*colourFade, colourFade*/);

	m_physicsScene->Draw();

}

void Game::GameEnd()
{
	if (p1Score > p2Score)
	{
		m_state = P1WIN;
	}
	else if (p1Score < p2Score)
	{
		m_state = P2WIN;
	}
	else
	{
		m_state = DRAW;
	}
}

void Game::GameRestart()
{
	m_state = MENU;
	ResetPositions();
	p1Score = 0;
	p2Score = 0;
	time = 60.f;
}

glm::vec2 Game::GetP1Position()
{
	return playerOne->GetPosition();
}

glm::vec2 Game::GetP2Position()
{
	return playerTwo->GetPosition();
}

void Game::ResetPositions()
{
	m_physicsScene->RemoveActor(playerOne);
	playerOne = new Box(playerOneSpawn, glm::vec2(0), 4.f, 10.f, 10.f, 0.1f, glm::vec4(0, 0, 1, 1));
	m_physicsScene->AddActor(playerOne);

	m_physicsScene->RemoveActor(playerTwo);
	playerTwo = new Box(playerTwoSpawn, glm::vec2(0), 4.f, 10.f, 10.f, 0.3f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(playerTwo);

	m_physicsScene->RemoveActor(ball);
	ball = new Sphere(ballSpawn, glm::vec2(0), 1.f, 4.f, 1.f, glm::vec4(1, 0.7f, 0, 1));
	m_physicsScene->AddActor(ball);
}

float Game::GetTime()
{
	return time;
}

int Game::GetP1Score()
{
	return p1Score;
}

int Game::GetP2Score()
{
	return p2Score;
}


