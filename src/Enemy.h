#pragma once
#ifndef __ENEMY__
#define __ENEMY__

#include "Agent.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "EnemyAnimationStates.h"
#include "Sprite.h"

class Enemy final : public Sprite
{
public:
	Enemy();
	~Enemy();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();

	// getters
	float getMaxSpeed() const;
	float getTurnRate() const;
	glm::vec2 getDesiredVelocity() const;
	float getAccelerationRate() const;
	float getHealth();

	// setters
	void setMaxSpeed(float newSpeed);
	void setTurnRate(float angle);
	void setDesiredVelocity(glm::vec2 target_position);
	void setAccelerationRate(float rate);
	void setHealth(float health);
	void setAnimationState(EnemyAnimationState new_state);

	// public member functions
	void Seek();
	void Arrival();
	void LookWhereIamGoing(glm::vec2 target_direction);
	
private:
	void m_buildAnimations();
	void m_checkBounds();
	void m_reset();
	void m_move();

	float m_health;
	float m_maxSpeed;
	float m_turnRate;
	glm::vec2 m_desiredVelocity;
	float m_accelerationRate;

	EnemyAnimationState m_currentAnimationState;
};


#endif /* defined (__ENEMY__) */

