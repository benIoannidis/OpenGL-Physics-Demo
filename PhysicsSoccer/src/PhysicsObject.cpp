#include "PhysicsObject.h"

int PhysicsObject::GetShapeID()
{
	return m_shapeID;
}

int PhysicsObject::GetPrimitiveCount()
{
	return LAST;
}