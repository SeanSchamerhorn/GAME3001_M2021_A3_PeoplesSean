#include "Radius.h"

#include "Util.h"

Radius::Radius()
{
	getRigidBody()->isColliding = false;
	setType(RADIUS);
}

Radius::~Radius() = default;

void Radius::draw()
{
	Util::DrawCircle(getTransform()->position, getWidth(),getRadiusColour());
}

void Radius::update()
{
}

void Radius::clean()
{
}

void Radius::setRadiusColour(glm::vec4 colour)
{
	m_radiusColour = colour;
}

glm::vec4 Radius::getRadiusColour()
{
	return m_radiusColour;
}


