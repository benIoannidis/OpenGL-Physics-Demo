#pragma once
#include "glm/vec2.hpp"
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);

	void Update(float dt);
	void Draw();

	void SetGravity(const glm::vec2 gravity); //set simulation gravity
	glm::vec2 GetGravity() const; //return gravity

	void SetTimeStep(const float timeStep); //set simulation timestep
	float GetTimeStep() const; //return timestep

	void CheckForCollision();

	static bool Sphere2Sphere(PhysicsObject*, PhysicsObject*); //determine if spheres are touching, and call sphere resolve collision to calculate force to be applied
	static bool Plane2Plane(PhysicsObject*, PhysicsObject*); //return false, planes are static and do not require their own checking
	static bool Sphere2Plane(PhysicsObject*, PhysicsObject*); //call sphere to plane
	static bool Plane2Sphere(PhysicsObject*, PhysicsObject*); //determine collision contact for sphere and plane, calls plane resolve collision to calculate force
	static bool Plane2Box(PhysicsObject*, PhysicsObject*); //find point on plane and check corners for intersect, if contact is occuring, call plane resolve collision to calculate force to be applie to box
	static bool Sphere2Box(PhysicsObject*, PhysicsObject*); //call box to sphere
	static bool Box2Plane(PhysicsObject*, PhysicsObject*); //call plane to box
	static bool Box2Sphere(PhysicsObject*, PhysicsObject*); //if sphere radius is inside box extents, call box resolve collision to calculate force to be applied to both actors
	static bool Box2Box(PhysicsObject*, PhysicsObject*); //check corners of boxes for overlap, if occuring, call box resolve collision to calculate force to be applied to both boxes

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

