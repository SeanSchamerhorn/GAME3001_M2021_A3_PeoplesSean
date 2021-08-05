#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "DisplayObject.h"

class Agent : public DisplayObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters

	// added to agent for movement
	float getAccelerationRate() const;
	float getMaxSpeed() const;

	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;

	glm::vec4 getLOSColour() const;
	float getLOSDistance() const;
	bool hasLOS() const;

	glm::vec4 getDetectionColour() const;
	float getDetectionRadius();
	bool hasDetection();

	float getCurrentHeading() const;

	float getTurnRate() const;

	// setters
	void setTargetPosition(glm::vec2 new_position);
	void setCurrentDirection(glm::vec2 new_direction);
	void setMaxSpeed(float newSpeed);
	void setAccelerationRate(float rate);
	
	// LOS
	void setLOSDistance(float distance);
	void setHasLOS(bool state);
	void setLOSColour(glm::vec4 colour);

	// Detection Radius
	void setDetectionRadius(float radius);
	void setHasDetection(bool state);
	void setDetectionColour(glm::vec4 colour);

	// Public member functions
	void Seek();
	//void LookWhereIamGoing(glm::vec2 target_direction);
	
	void setCurrentHeading(float heading);


	void m_movement();

private:
	void m_changeDirection();

	
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	
	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	// Radius
	float m_DetectionRadius;
	bool m_isWithinRadius;
	glm::vec4 m_DetectColour;


	float m_maxSpeed;
	float m_accelerationRate;

	
	
};



#endif /* defined ( __AGENT__) */