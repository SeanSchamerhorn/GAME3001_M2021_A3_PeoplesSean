#pragma once
#ifndef __HEALTHBAR__
#define __HEALTHBAR__
#include "DisplayObject.h"

class HealthBar : public DisplayObject
{
public:
	// Constructor
	HealthBar();

	// Destructor
	~HealthBar();

	// Life Cycle functions Inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;
private:

};
#endif