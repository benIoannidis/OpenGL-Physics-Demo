#pragma once
#include "Rigidbody.h"
#include "glm/vec4.hpp"

class Sphere :
    public Rigidbody
{
public:
    Sphere(glm::vec2 position, glm::vec2 velocity, 
        float mass, float radius, float elasticity, glm::vec4 colour);
    ~Sphere() {}

    virtual void Draw(); //draw sphere

    float GetRadius(); //return radius

    glm::vec4 GetColour(); //return colour

protected:
    float m_radius;
    glm::vec4 m_colour;
};

