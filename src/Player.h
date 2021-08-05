#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "HealthBar.h"
#include "PlayerAnimationState.h"
#include "Radius.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);

	//getters
	float getTurnRate() const;
	
	// setters
	double GetVelX();
	double GetVelY();
	void SetX(float y);
	void SetY(float y);
	void setTurnRate(float angle);
	
	void setAnimationState(PlayerAnimationState new_state);

	void LookWhereIamGoing(glm::vec2 target_direction);
	
	Radius* m_CollisionRadius;
	HealthBar* m_healthBar;

private:
	void m_buildAnimations();

	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY;

	float m_turnRate;

	float m_health;
	
	PlayerAnimationState m_currentAnimationState;
};

#endif /* defined (__PLAYER__) */