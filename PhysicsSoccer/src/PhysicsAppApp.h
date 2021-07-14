#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "glm/vec2.hpp"

class Game;
class PhysicsScene;
class Sphere;

class PhysicsAppApp : public aie::Application {
public:

	PhysicsAppApp();
	virtual ~PhysicsAppApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	virtual glm::vec2 ScreenToWorld(glm::vec2 screenPos);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Game* runningGame; //instance of my game class

	const float extents = 100;
	const float aspectRatio = 16.f / 9.f;
};