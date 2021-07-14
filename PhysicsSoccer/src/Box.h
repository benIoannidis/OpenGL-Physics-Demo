#pragma once
#include "Rigidbody.h"
#include "glm/vec4.hpp"

class Box :
    public Rigidbody
{
public:
    Box(glm::vec2 position, glm::vec2 velocity, 
        float mass, float width, float height, float elasticity, glm::vec4 colour);
    ~Box() {}

    void FixedUpdate(glm::vec2 gravity, float timeStep); //call rigidbody update for velocity and angular velocity, then calculate rotation
    virtual void Draw();

    glm::vec2 GetExtents(); //return extents

    float GetWidth(); //return width
    float GetHeight(); //return height

    glm::vec2 GetLocalX(); //return localX
    glm::vec2 GetLocalY(); //return localY

    bool CheckBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal); //check corners of box, check for seperating axes, if none, find penetration and return true if it exists

    void SetOrientation(float orientation); //set boxe's orientation

    void NullAngularVelocity(); //reset angular velocity to 0
protected:
    glm::vec2 m_extents;
    glm::vec4 m_colour;

    //local x,y axes of box based on angle of rotation
    glm::vec2 m_localX;
    glm::vec2 m_localY;
};

