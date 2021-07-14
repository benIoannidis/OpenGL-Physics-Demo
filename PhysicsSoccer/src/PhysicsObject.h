#pragma once
#include "glm/vec2.hpp"

enum ShapeType {
	PLANE = 0,
	SPHERE,
	BOX,
	LAST
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	ShapeType m_shapeID;

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};

	int GetShapeID(); //return objects shape ID

	int GetPrimitiveCount(); //return last item in shapetype enum
};

