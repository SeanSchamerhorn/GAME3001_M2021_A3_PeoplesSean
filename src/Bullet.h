#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "Sprite.h"
#include "BulletAnimationState.h"
class Bullet : public Sprite
{
public:
	// Constructor
	Bullet();

	// Destructor
	~Bullet();

	// Life Cycle functions Inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;


	void StopX();
	void StopY();
	void SetAccelX(float a);
	void SetAccelY(float a);

	// setters
	float GetVelX();
	float GetVelY();

	// getters
	void SetX(float y);
	void SetY(float y);
	
	void setAnimationState(BulletAnimationState new_state);
	

private:
	void m_buildAnimations();

	float m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY;
	
	BulletAnimationState m_currentAnimationState;
};
#endif