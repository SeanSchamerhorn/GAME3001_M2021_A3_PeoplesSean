#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "NavigationObject.h"
#include "Radius.h"

class Obstacle final : public NavigationObject
{
public:
	// constructors
    Obstacle();
	
	// destructor
	~Obstacle();
	
	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	Radius* m_CollisionRadius;
private:
	 
};

#endif /* defined (__OBSTACLE__) */