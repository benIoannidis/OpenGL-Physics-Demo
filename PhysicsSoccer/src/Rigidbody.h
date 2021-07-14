#pragma once
#include "glm/vec2.hpp"


#include "PhysicsObject.h"
class Rigidbody :
    public PhysicsObject
{
public:
    Rigidbody(ShapeType shapeID, glm::vec2 position,
        glm::vec2 velocity, float orientation, float mass, float angularVelocity, float momentum, float elasticity);
    ~Rigidbody() {};

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

    void ApplyForce(glm::vec2 force, glm::vec2 pos); //apply force/mass to velocity, and force at a position/momentum to angular velocity
    void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);

    glm::vec2 GetPosition(); //return pos
    float GetOrientation(); //retrun orientation
    glm::vec2 GetVelocity(); //return velocity
    float GetMass(); //return mass
    float GetMoment(); //return momentum
    float GetAngularVelocity(); //return angular velocity

    float GetLinearDrag(); //return linear drag
    void SetLinearDrag(float);

    float GetAngularDrag(); //return angular drag
    void SetAngularDrag(float);

    float GetElasticity(); //return elasticity

    void NullVelocity(); //reset rigidbody's velocity to (0, 0)

    void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr); //calculate force based on actor and contact, then apply resulting force to actor

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;
    float m_angularVelocity;
    float m_moment;

    float m_linearDrag = 0.3f;
    float m_angularDrag = 0.8f;

    float m_elasticity;
};

