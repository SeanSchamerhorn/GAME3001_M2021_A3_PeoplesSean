#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance().load("../Assets/textures/Rock.png", "obstacle");

	auto size = TextureManager::Instance().getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;
	
	m_CollisionRadius = new Radius();
	m_CollisionRadius->setWidth(20.0f);
	m_CollisionRadius->setRadiusColour({ 1,0,1,255 });
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{/*
	TextureManager::Instance().draw("obstacle",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);*/
	
	m_CollisionRadius->draw();
}

void Obstacle::update()
{
	//auto offset = glm::vec2()
	m_CollisionRadius->getTransform()->position = glm::vec2(this->getTransform()->position);
}

void Obstacle::clean()
{
}
