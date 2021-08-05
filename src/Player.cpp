#include "Player.h"
#include "TextureManager.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE_DOWN)
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
	setType(PLAYER);

	m_buildAnimations();

	//m_healthBar = new HealthBar();
	//m_healthBar->setWidth(40.0f);
	//m_healthBar->setHeight(10.0f);

	m_CollisionRadius = new Radius();
	m_CollisionRadius->setWidth(20.0f);

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelY = 10.0;
	m_maxVelX = 10.0;
	
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_DOWN:
			TextureManager::Instance().playAnimation("spritesheet", getAnimation("idle"),
				x, y, 0.03f, 0, 255, true);
		break;
	case PLAYER_CLOSE_ATTACK:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("swing"),
			x, y, 1.0f, 0, 255, true);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.5f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.5f, 0, 255, true);
		break;
	case PLAYER_RUN_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("runDown"),
			x, y, 0.5f, 0, 255, true);
		break;
	case PLAYER_RUN_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("runUp"),
			x, y, 0.5f, 0, 255, true);
		break;
	default:
		break;
	}

	m_CollisionRadius->draw();
	//m_healthBar->draw();
}

void Player::update()
{   // X axis
	m_velX += m_accelX;
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	
	this->getTransform()->position.x += m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Y axis
	m_velY += m_accelY; 
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_maxVelY));

	this->getTransform()->position.y += m_velY; 
	m_accelX = m_accelY = 0.0;
	StopX();
	StopY();

	m_CollisionRadius->getTransform()->position = glm::vec2(this->getTransform()->position);
	//m_healthBar->getTransform()->position = glm::vec2(this->getTransform()->position);
}

void Player::clean()
{
}

void Player::StopX(){m_velX = 0.0;}
void Player::StopY() { m_velY = 0.0; }


void Player::SetAccelX(double a) { m_accelX = a; }
void Player::SetAccelY(double a) { m_accelY = a; }

double Player::GetVelX() { return m_velX; }
double Player::GetVelY() { return m_velY; }


void Player::SetX(float y) { this->getTransform()->position.x = y; }
void Player::SetY(float y) { this->getTransform()->position.y = y; }

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation runDownAnimation = Animation();

	runDownAnimation.name = "runDown";
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-0"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-1"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-2"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-3"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-4"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-5"));

	setAnimation(runDownAnimation);


	Animation runUpAnimation = Animation();

	runUpAnimation.name = "runUp";
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-0"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-1"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-2"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-3"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-4"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-5"));

	setAnimation(runUpAnimation);

	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	//idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-2"));
	

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-4"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-5"));

	setAnimation(runAnimation);

	Animation swingAnimation = Animation();

	swingAnimation.name = "swing";
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-0"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-1"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-2"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-3"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-4"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-5"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-6"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-7"));

	setAnimation(swingAnimation);
}
