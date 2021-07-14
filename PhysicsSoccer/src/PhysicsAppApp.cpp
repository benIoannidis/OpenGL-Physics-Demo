#include "PhysicsAppApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "PhysicsScene.h"
#include "glm/ext.hpp"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Game.h"

#include <iostream>

PhysicsAppApp::PhysicsAppApp() {
	
}

PhysicsAppApp::~PhysicsAppApp() {

}

bool PhysicsAppApp::startup() {
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	//set to green
	setBackgroundColour(0.2f, 0.5f, 0.2f, 1.0f);

	//instantiate game
	runningGame = new Game(glm::vec2(-20, 0), glm::vec2(20, 0), glm::vec2(0), 55, 90);

	return true;
}

void PhysicsAppApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsAppApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//check menu buttons clicked
	if (input->isMouseButtonDown(0) && runningGame->CheckState() == MENU)
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);

		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

		if ((worldPos.x >= -12 && worldPos.x <= 22) && (worldPos.y <= 17 && worldPos.y >= 10))
		{
			runningGame->GameStart();
		}
		else if ((worldPos.x >= -12 && worldPos.x <= 22) && (worldPos.y <= -10 && worldPos.y >= -17))
		{
			quit();
		}
	}

	//update game
	runningGame->Update(deltaTime);

	//draw game
	runningGame->Draw();


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}
}

void PhysicsAppApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	

	// draw your stuff here!
	aie::Gizmos::draw2D(glm::ortho<float>(-extents, extents,
				-extents / aspectRatio, extents / aspectRatio, -1.0f, 1.0f));

	
	
	// output some text, uses the last used colour
	m_2dRenderer->setRenderColour(1.f, 1.f, 1.f, 1.f);

	//draw fps and "escape"
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 5, 645);

	//draw game time
	if (runningGame->GetTime() > 0.f)
	{
		char time[32];
		sprintf_s(time, 32, "Time: %i", (int)runningGame->GetTime());
		m_2dRenderer->drawText(m_font, time, 550, 680);
	}

	//check state and draw objects accordingly
	GameState currentState = runningGame->CheckState();
	if (currentState == RUNNING)
	{
		char p1Score[32];
		char p2Score[32];

		sprintf_s(p1Score, 32, "P1: %i", runningGame->GetP1Score());
		m_2dRenderer->setRenderColour(0.f, 0.f, 1.f, 1.f);
		m_2dRenderer->drawText(m_font, p1Score, 400, 680);

		sprintf_s(p2Score, 32, "P2: %i", runningGame->GetP2Score());
		m_2dRenderer->setRenderColour(1.f, 0.f, 0.f, 1.f);
		m_2dRenderer->drawText(m_font, p2Score, 750, 680);
		
	}
	else 
	{
		switch (currentState)
		{
		case P1SCORED:
			//playerone scored
			m_2dRenderer->drawBox(625, 460, 360, 50);
			m_2dRenderer->setRenderColour(0.f, 0.f, 1.f, 1.f);
			m_2dRenderer->drawText(m_font, "PLAYER ONE SCORES!", 475, 450);
			break;

		case P2SCORED:
			//playertwo scored
			m_2dRenderer->drawBox(625, 460, 360, 50);
			m_2dRenderer->setRenderColour(1.f, 0.f, 0.f, 1.f);
			m_2dRenderer->drawText(m_font, "PLAYER TWO SCORES!", 475, 450);
			break;

		case NOTIME:
			//game time finished
			break;

		case MENU:
			//640 centre X -_-_- 360 centre Y
			m_2dRenderer->setRenderColour(0.7f, 0.7f, 0.7f, 0.6f);
			m_2dRenderer->drawBox(640, 360, 1280, 720);
			m_2dRenderer->setRenderColour(0, 1, 0, 1);
			//green play box
			m_2dRenderer->drawBox(645, 450, 175, 50);
			//red exit box
			m_2dRenderer->setRenderColour(1, 0, 0, 1);
			m_2dRenderer->drawBox(645, 270, 175, 50);

			//button texts
			m_2dRenderer->setRenderColour(1, 1, 1, 1);
			m_2dRenderer->drawText(m_font, "PLAY", 610, 440);
			m_2dRenderer->drawText(m_font, "EXIT", 610, 260);


			//controls & game info
			m_2dRenderer->setRenderColour(1, 0, 1, 1);
			m_2dRenderer->drawText(m_font, "BEST OF 5", 570, 600);
			m_2dRenderer->setRenderColour(0, 0, 1, 1);
			m_2dRenderer->drawText(m_font, "W - up", 250, 440);
			m_2dRenderer->drawText(m_font, "A - left", 250, 400);
			m_2dRenderer->drawText(m_font, "S - down", 250, 360);
			m_2dRenderer->drawText(m_font, "D - right", 250, 320);
			m_2dRenderer->drawText(m_font, "Q - rotate left", 250, 280);
			m_2dRenderer->drawText(m_font, "E - rotate right", 250, 240);

			m_2dRenderer->setRenderColour(1, 0, 0, 1);
			m_2dRenderer->drawText(m_font, "UP - up", 780, 440);
			m_2dRenderer->drawText(m_font, "LEFT - left", 780, 400);
			m_2dRenderer->drawText(m_font, "DOWN - down", 780, 360);
			m_2dRenderer->drawText(m_font, "RIGHT - right", 780, 320);
			m_2dRenderer->drawText(m_font, "< - rotate left", 780, 280);
			m_2dRenderer->drawText(m_font, "> - rotate right", 780, 240);
			break;

		case P1WIN:
			m_2dRenderer->drawBox(625, 460, 360, 50);
			m_2dRenderer->setRenderColour(0.f, 0.f, 1.f, 1.f);
			m_2dRenderer->drawText(m_font, "PLAYER ONE WINS!", 475, 450);
			break;

		case P2WIN:
			m_2dRenderer->drawBox(625, 460, 360, 50);
			m_2dRenderer->setRenderColour(1.f, 0.f, 0.f, 1.f);
			m_2dRenderer->drawText(m_font, "PLAYER TWO WINS!", 475, 450);
			break;

		case DRAW:
			m_2dRenderer->drawBox(625, 460, 360, 50);
			m_2dRenderer->setRenderColour(1.f, 0.f, 1.f, 1.f);
			m_2dRenderer->drawText(m_font, "IT'S A DRAW!", 520, 450);
			break;

		default:
			break;
		}
	}

	// done drawing sprites
	m_2dRenderer->end();
}

//change screen coords to world coords for mouse use
glm::vec2 PhysicsAppApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	//move the centre to (0, 0)
	worldPos.x -= getWindowWidth() * .5f;
	worldPos.y -= getWindowHeight() * .5f;

	//scale to extents
	worldPos.x *= 2.f * extents / getWindowWidth();
	worldPos.y *= 2.f * extents / (aspectRatio * getWindowHeight());

	return worldPos;
}