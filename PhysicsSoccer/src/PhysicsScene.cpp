#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "glm/glm.hpp"
#include "Box.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		CheckForCollision();
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw();
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
			break;
		}
	}
}

//function pointer array for collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn CollisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box,
};


void PhysicsScene::CheckForCollision()
{
	int actorCount = (int)m_actors.size();

	for (int i = 0; i < actorCount - 1; i++)
	{
		for (int j = i + 1; j < actorCount; j++)
		{
			PhysicsObject* object1 = m_actors[i];
			PhysicsObject* object2 = m_actors[j];

			int shape1ID = object1->GetShapeID();
			int shape2ID = object2->GetShapeID();

			int shapeCount = object1->GetPrimitiveCount();

			//function pointers
			int functionIDx = (shape1ID * shapeCount) + shape2ID;
			fn collisionFunctionPtr = CollisionFunctionArray[functionIDx];
			if (collisionFunctionPtr != nullptr)
			{
				//did collision occur
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}


bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	//if successful
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float xDis = sphere1->GetPosition().x - sphere2->GetPosition().x;
		float yDis = sphere1->GetPosition().y - sphere2->GetPosition().y;

		float hypot = (float)sqrt((xDis * xDis) + (yDis * yDis));
		if ((sphere1->GetRadius() + sphere2->GetRadius()) > hypot)
		{
			sphere1->ResolveCollision(sphere2, (sphere1->GetPosition() + sphere2->GetPosition()) * 0.5f);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
}

bool PhysicsScene::Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//planes are static and require no response
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		float sphereToPlane = glm::dot(sphere->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = sphere->GetRadius() - sphereToPlane;

		float velocityOutOfPlane = glm::dot(sphere->GetVelocity(), plane->GetNormal());
		
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			glm::vec2 contact = sphere->GetPosition() + (plane->GetNormal() * -sphere->GetRadius());
			plane->ResolveCollision(sphere, contact);
			return true;
		}
		return false;
	}
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Plane2Box(obj2, obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);

	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;

		//get point on the plane
		glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

		//check 4 corners for collision with plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				//get pos of corner in world space
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distanceFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

				//total velocity of point in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() *
					glm::vec2(-displacement.y, displacement.x);

				//component of point velocity into plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());


				//moving further in, resolve collision
				if (distanceFromPlane <= 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}

		//on hit, only 2 corners can contact 
		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}

	//no collision
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Box2Sphere(obj2, obj1);
}

bool PhysicsScene::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	if (box != nullptr && sphere != nullptr)
	{
		//transform circle into box's coordinate space
		glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

		//find closest point to circle centre on box by clamping coordinates in box-space to box's extents
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();

		if (closestPointOnBoxBox.x < -extents.x)
		{
			closestPointOnBoxBox.x = -extents.x;
		}

		if (closestPointOnBoxBox.x > extents.x)
		{
			closestPointOnBoxBox.x = extents.x;
		}

		if (closestPointOnBoxBox.y < -extents.y)
		{
			closestPointOnBoxBox.y = -extents.y;
		}
		if (closestPointOnBoxBox.y > extents.y)
		{
			closestPointOnBoxBox.y = extents.y;
		}

		//convert back to world coordinates
		glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x *
			box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();

		glm::vec2 circleToBox = sphere->GetPosition() - closestPointOnBoxWorld;

		//check for collision
		if (glm::length(circleToBox) < sphere->GetRadius())
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(sphere, contact, &direction);
		}
	}

	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();

		glm::vec2 norm(0);
		glm::vec2 contact(0);
		float pen = 0;
		int numContacts = 0;

		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm;
		}
		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / (float)numContacts, &norm);
		}
		return true;
	}
	return false;
}

void PhysicsScene::SetGravity(const glm::vec2 gravity)
{
	m_gravity = gravity;
}

glm::vec2 PhysicsScene::GetGravity() const
{
	return m_gravity;
}

void PhysicsScene::SetTimeStep(const float timeStep)
{
	m_timeStep = timeStep;
}

float PhysicsScene::GetTimeStep() const
{
	return m_timeStep;
}
