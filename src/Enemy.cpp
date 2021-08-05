#include "Enemy.h"
#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Enemy::Enemy() : m_maxSpeed(10.0f), m_currentAnimationState(ENEMY_WALK_RIGHT)
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
	m_turnRate = 3.0f;
	m_accelerationRate = 5.0f;

	setLOSDistance(200.0f);

	setDetectionRadius(250.0f);
	
}

Enemy::~Enemy()
= default;

void Enemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw LOS
	Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());

	// draw Detection Radius
	Util::DrawCircle(getTransform()->position, getDetectionRadius(), getDetectionColour());

	switch (m_currentAnimationState)
	{
	case ENEMY_WALK_UP :
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkUp"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkDown"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_LEFT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkLeft"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_RIGHT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkRight"),
			x, y, 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case ENEMY_IDLE_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idleUp"),
			x, y, 0.03f, 0, 255, true);
		break;
	case ENEMY_IDLE_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idleDown"),
			x, y, 0.02f, 0, 255, true);
		break;
	case ENEMY_IDLE_LEFT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idleLeft"),
			x, y, 0.03f, 0, 255, true);
		break;
	case ENEMY_IDLE_RIGHT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idleRight"),
			x, y, 0.03f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
		
	}
}

void Enemy::update()
{
	m_move();
	Arrival();
	
	//m_checkBounds();
}

void Enemy::clean()
{
}

void Enemy::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Enemy::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Enemy::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Enemy::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Enemy::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

float Enemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Enemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}
float Enemy::getTurnRate() const
{
	return m_turnRate;
}

void Enemy::setTurnRate(float angle)
{
	m_turnRate = angle;
}

glm::vec2 Enemy::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

float Enemy::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Enemy::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void Enemy::setDesiredVelocity(glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position) * m_maxSpeed;
	getRigidBody()->velocity = m_desiredVelocity - getRigidBody()->velocity;
}

void Enemy::setAnimationState(EnemyAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Enemy::Seek()
{
	// compute the target direction and magnitude
	auto target_direction = getTargetPosition() - this->getTransform()->position;

	// normalize the target direction
	target_direction = Util::normalize(target_direction);

	

	// seek with LookWhereIamGoing
	LookWhereIamGoing(target_direction);

	// seek without LookWhereIamGoing
	//setCurrentDirection(target_direction); 
}

void Enemy::Arrival()
{
	//Seek();
	auto target_distance = Util::distance(getTransform()->position, getTargetPosition());
	auto target_direction = getTransform()->position - getTargetPosition();
	Util::normalize(target_direction);
	//std::cout << "Enemy Pos: (" << getTransform()->position.x << ", " << getTransform()->position.y << ")" << std::endl;
	//std::cout << "Target Pos: (" << getTargetPosition().x << ", " << getTargetPosition().y << ")" << std::endl;

	if (target_distance <= 200.0f)
	{	
		if (target_distance < 1)
		{
			setMaxSpeed(0.0f);
			setAnimationState(ENEMY_IDLE_DOWN);
		}
		setMaxSpeed(getMaxSpeed() / 1.003f );
	}
	else
	{
		setMaxSpeed(1.0f);
	}
}

void Enemy::LookWhereIamGoing(glm::vec2 target_direction)
{
	const auto target_rotation = Util::signedAngle(getCurrentDirection(), target_direction);

	const auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setCurrentHeading(getCurrentHeading() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setCurrentHeading(getCurrentHeading() - getTurnRate());
		}
	}
}

void Enemy::m_checkBounds()
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

void Enemy::m_move()
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

void Enemy::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Enemy::m_buildAnimations()
{
	Animation walkUpAnimation = Animation();
	walkUpAnimation.name = "walkUp";
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-0"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-1"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-2"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-3"));
	setAnimation(walkUpAnimation);

	Animation walkDownAnimation = Animation();
	walkDownAnimation.name = "walkDown";
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-0"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-1"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-2"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-3"));
	setAnimation(walkDownAnimation);

	Animation walkLeftAnimation = Animation();
	walkLeftAnimation.name = "walkLeft";
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-0"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-1"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-2"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-3"));
	setAnimation(walkLeftAnimation);

	Animation walkRightAnimation = Animation();
	walkRightAnimation.name = "walkRight";
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-0"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-1"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-2"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-3"));
	setAnimation(walkRightAnimation);

	Animation idleUpAnimation = Animation();
	idleUpAnimation.name = "idleUp";
	idleUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-up-0"));
	idleUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-up-1"));
	idleUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-up-2"));
	idleUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-up-3"));
	setAnimation(idleUpAnimation);

	Animation idleDownAnimation = Animation();
	idleDownAnimation.name = "idleDown";
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-0"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-1"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-2"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-3"));
	setAnimation(idleDownAnimation);

	Animation idleLeftAnimation = Animation();
	idleLeftAnimation.name = "idleLeft";
	idleLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-0"));
	idleLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-1"));
	idleLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-2"));
	idleLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-3"));
	setAnimation(idleLeftAnimation);

	Animation idleRightAnimation = Animation();
	idleRightAnimation.name = "idleRight";
	idleRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-0"));
	idleRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-1"));
	idleRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-2"));
	idleRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-left-3"));
	setAnimation(idleRightAnimation);
}