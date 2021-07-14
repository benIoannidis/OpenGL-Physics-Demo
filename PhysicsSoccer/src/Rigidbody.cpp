#include "Rigidbody.h"
#include "glm/glm.hpp"
#include <iostream>

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ANGULAR_THRESHOLD 0.01f

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position,
    glm::vec2 velocity, float orientation, float mass, float angularVelocity, float momentum, float elasticity) : PhysicsObject(shapeID)
{
    m_position = position;
    m_velocity = velocity;
    m_orientation = orientation;
    m_mass = mass;
    m_angularVelocity = angularVelocity;
    m_moment = momentum;
    m_elasticity = elasticity;
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
    m_position += m_velocity * m_linearDrag * timeStep;
    ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));
    m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
    m_orientation += m_angularVelocity * timeStep;

    if (length(m_velocity) < MIN_LINEAR_THRESHOLD || (length(m_velocity) > -MIN_LINEAR_THRESHOLD && length(m_velocity) < 0))
    {
        m_velocity = glm::vec2(0);
    }
    if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
    {
        m_angularVelocity = 0;
    }
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
    m_velocity += force / GetMass();
    m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}

void Rigidbody::ApplyForceToActor(Rigidbody* actor2, glm::vec2 force)
{
    actor2->ApplyForce(-force, m_position);
    this->ApplyForce(force, m_position);
}

void Rigidbody::NullVelocity()
{
    m_velocity = glm::vec2(0, 0);
}

void Rigidbody::SetLinearDrag(float drag)
{
    m_linearDrag = drag;
}

void Rigidbody::SetAngularDrag(float drag)
{
    m_angularDrag = drag;
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
    glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
        actor2->m_position - m_position);

    glm::vec2 perp(normal.y, -normal.x);


    float r1 = glm::dot(contact - m_position, -perp);
    float r2 = glm::dot(contact - actor2->m_position, perp);

    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
    float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

    if (v1 > v2)//they're moving closer
    {
        float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
        float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);
        
        float elasticity =(GetElasticity() + actor2->GetElasticity()) / 2.0f;
       
        /*glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
            (mass1 + mass2) * (v1 - v2) * normal;*/
        glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;
        float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
            glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->GetMass())));

        //glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;
        glm::vec2 force = normal * j;
        //apply equal and opposite forces
        ApplyForce(-force, contact - m_position);
        actor2->ApplyForce(force, contact - actor2->m_position);
    }
}

glm::vec2 Rigidbody::GetPosition()
{
    return m_position;
}

float Rigidbody::GetOrientation()
{
    return m_orientation;
}

glm::vec2 Rigidbody::GetVelocity()
{
    return m_velocity;
}

float Rigidbody::GetMass()
{
    return m_mass;
}

float Rigidbody::GetMoment()
{
    return m_moment;
}

float Rigidbody::GetAngularVelocity()
{
    return m_angularVelocity;
}

float Rigidbody::GetLinearDrag()
{
    return m_linearDrag;
}

float Rigidbody::GetAngularDrag()
{
    return m_angularDrag;
}

float Rigidbody::GetElasticity()
{
    return m_elasticity;
}