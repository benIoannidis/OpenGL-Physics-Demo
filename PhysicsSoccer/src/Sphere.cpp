#include "Sphere.h"
#include "Gizmos.h"
#include <cmath>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity,
    float mass, float radius, float elasticity, glm::vec4 colour) :
    Rigidbody(SPHERE, position, velocity, 0.5, mass, 0.0001f, 0.0001f, elasticity)
{
    m_radius = radius;
    m_colour = colour;

    m_moment = 0.5f * m_mass * m_radius * m_radius;
}

void Sphere::Draw()
{
    glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;
    aie::Gizmos::add2DCircle(m_position, m_radius, 1000, m_colour);
}

float Sphere::GetRadius() 
{ 
    return m_radius; 
}

glm::vec4 Sphere::GetColour() 
{ 
    return m_colour; 
}