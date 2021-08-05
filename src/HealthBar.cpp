#include "HealthBar.h"

#include "Util.h"


HealthBar::HealthBar()
{
	
	setWidth(40.0f);
	setHeight(10.0f);
}

HealthBar::~HealthBar()
= default;

void HealthBar::draw()
{
	Util::DrawFilledRect(this->getTransform()->position - glm::vec2(20.0f,35.0f), getWidth(), getHeight(),{0.0f,0.0f,1.0f,1.0f});
}

void HealthBar::update()
{
}

void HealthBar::clean()
{
}