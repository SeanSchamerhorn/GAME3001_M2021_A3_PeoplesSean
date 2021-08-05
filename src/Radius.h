#pragma once
#ifndef __RADIUS__
#define __RADIUS__
#include <glm/detail/type_vec4.hpp>

#include "DisplayObject.h"

class Radius : public DisplayObject
{
public:
	// constructor
	Radius();

	// destructor
	~Radius();

	// lifecycle functions
	void draw() override;
	void update() override;
	void clean() override;


	void setRadiusColour(glm::vec4 colour);
	glm::vec4 getRadiusColour();

private:

	glm::vec4 m_radiusColour;
};


#endif /*(defined __RADIUS__)*/
