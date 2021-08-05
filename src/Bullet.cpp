#include "Bullet.h"

#include "Game.h"
#include "TextureManager.h"

Bullet::Bullet(): m_currentAnimationState(BULLET_UP)
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
	setType(BILL);

	m_buildAnimations();

	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelY = 10.0;
	m_maxVelX = 10.0;
}

Bullet::~Bullet()
= default;


void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	
	switch(m_currentAnimationState)
	{
	case BULLET_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("bulletUp"),
			x, y, 0.5f, 0, 255, true);
		break;
	case BULLET_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("bulletUp"),
			x, y, 0.5f, 0, 255, true,SDL_FLIP_VERTICAL);
		break;
	}
	
}

void Bullet::update()
{
	// X axis
	m_velX += m_accelX;
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	this->getTransform()->position.x += m_velX; 

	// Y axis
	m_velY += m_accelY;
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_maxVelY));
	this->getTransform()->position.y += m_velY;

	/*m_accelX = m_accelY = 0.0;
	StopX();
	StopY();*/
	//
	//if(this->getTransform()->position.y > 300.0f)
	//{
	//	this->getTransform()->position.y++;
	//}
	//else if(this->getTransform()->position.y < 300.0f)
	//{
	//	this->getTransform()->position.y--;
	//}
	///*if(TheGame::Instance().getMousePosition().y <=300.0f)
	//{
	//	this->getTransform()->position.y++;
	//}*/
}

void Bullet::clean()
{
}

void Bullet::StopX() { m_velX = 0.0; }
void Bullet::StopY() { m_velY = 0.0; }


void Bullet::SetAccelX(float a) { m_accelX = a; }
void Bullet::SetAccelY(float a) { m_accelY = a; }
	 
float Bullet::GetVelX() { return m_velX; }
float Bullet::GetVelY() { return m_velY; }


void Bullet::SetX(float y) { this->getTransform()->position.x = y; }
void Bullet::SetY(float y) { this->getTransform()->position.y = y; }

void Bullet::setAnimationState(BulletAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Bullet::m_buildAnimations()
{
	Animation bulletAnimation = Animation();
	bulletAnimation.name = "bulletUp";
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-0"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-1"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-2"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-3"));

	setAnimation(bulletAnimation);
}