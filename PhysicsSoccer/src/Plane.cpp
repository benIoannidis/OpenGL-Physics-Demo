#include "Plane.h"
#include "Gizmos.h"
#include "Rigidbody.h"
#include "glm/glm.hpp"
#include <iostream>

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = glm::vec4(0.75f, 1, 0.75f, 1);
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::Draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	//easy to rotate normal through 90o around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	//glm::vec4 colourFade = m_colour;
	//colourFade.a = 0;

	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

	aie::Gizmos::add2DTri(start, end, start - m_normal * 2.0f, m_colour, m_colour, m_colour/*, colourFade*/);

	aie::Gizmos::add2DTri(end, end - m_normal * 2.0f, start - m_normal * 2.0f, m_colour, m_colour, m_colour/*colourFade, colourFade*/);

}

void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	//position to apply force relative to objects COM
	glm::vec2 localContact = contact - actor2->GetPosition();

	//plane isn't moving, relative velocity is actor2's at contact point
	glm::vec2 relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	
	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);

	float elasticity = actor2->GetElasticity();
	
	//perpendicular distance we apply force at relative to COM, (Torque = F*r)
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));


	//find effective mass, combine moment of inertia and mass,
	//shows how much contact point velocity will change when force is applied
	float tempMoment = actor2->GetMoment();
	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + elasticity) * velocityIntoPlane * mass0;
	
	glm::vec2 force = m_normal * j;
	
	actor2->ApplyForce(force, localContact);
}

glm::vec2 Plane::GetNormal()
{
	return m_normal;
}

float Plane::GetDistance()
{
	return m_distanceToOrigin;
}
