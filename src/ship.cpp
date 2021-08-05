#include "ship.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Ship::Ship() : m_maxSpeed(10.0f)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/BlueLinkAtlas.png",
		"spritesheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("spritesheet"));
	
	// set frame width
	setWidth(40);
	// set frame height
	setHeight(40);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);

	m_buildAnimations();

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	m_maxSpeed = 1.0f;
	m_turnRate = 10.0f; 
	m_accelerationRate = 5.0f;

	setLOSDistance(300.0f); 

	setDetectionRadius(500.0f);

}


Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
}


void Ship::update()
{	
	//m_move();
	//m_checkBounds();
}

void Ship::clean()
{
}

void Ship::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Ship::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Ship::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Ship::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Ship::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

float Ship::getMaxSpeed() const
{
	return m_maxSpeed;
}

float Ship::getTurnRate() const
{
	return m_turnRate;
}

glm::vec2 Ship::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

float Ship::getAccelerationRate() const
{
	return m_accelerationRate;
}

float Ship::getHealth()
{
	return m_health;
}

void Ship::setHealth(float health)
{
	m_health = health;
}

void Ship::setTurnRate(float angle)
{
	m_turnRate = angle;
}

void Ship::setDesiredVelocity(glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position) * m_maxSpeed;
	getRigidBody()->velocity = m_desiredVelocity - getRigidBody()->velocity;
}

void Ship::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Ship::Seek()
{
	// compute the target direction and magnitude
	auto target_direction = getTargetPosition() - getTransform()->position;

	// normalize the target direction
	target_direction = Util::normalize(target_direction);

	//std::cout << "Target Direction: (" << target_direction.x << ", " << target_direction.y << ")" << std::endl;

	// seek with LookWhereIamGoing
	//LookWhereIamGoing(target_direction);

	// seek without LookWhereIamGoing
	//setCurrentDirection(target_direction); 
}

void Ship::Arrival()
{
	Seek();
	auto target_distance = Util::distance(getTransform()->position, getTargetPosition());
	if (target_distance <= 200)
	{
		setMaxSpeed((getMaxSpeed() / 1.02f));
	}
	else
	{
		setMaxSpeed(3.0f);
	}
}



void Ship::setAnimationState(EnemyAnimationState new_state)
{
}

void Ship::m_buildAnimations()
{
}

void Ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Ship::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Ship::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Ship::m_move()
{
	auto deltaTime = TheGame::Instance().getDeltaTime();

	getRigidBody()->acceleration = getCurrentDirection() * getAccelerationRate();

	// use kinematic equation -- pf = pi + vi * t + 0.5ai * t^2

	// compute velocity and acceleration terms
	getRigidBody()->velocity += getCurrentDirection() * (deltaTime)+0.5f * getRigidBody()->acceleration * (deltaTime);

	// clamp velocity to maxSpeed
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());

	// add velocity to position
	getTransform()->position += getRigidBody()->velocity;
}
