#include <Jumping.h>
#include <Climbing.h>
#include "Falling.h"

#include <string>

Jumping::Jumping()
{
	m_clock.restart();
}

void Jumping::handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input)
{
	if (t_input.m_upPressed) // If the up key is pressed move to climbing state
	{
		t_animatedSprite->changeAnimation(Animation::Climbing);
		climbing(a);
	}
}

void Jumping::update(PlayerFSM* a, AnimatedSprite* t_animatedSprite)
{
	if (m_clock.getElapsedTime().asSeconds() > 1.0f) // Check if two seconds has passed since started jumping
	{
		t_animatedSprite->changeAnimation(Animation::Falling);
		falling(a);
	}
}

void Jumping::falling(PlayerFSM* a)
{
	std::cout << "Jumping -> Falling" << std::endl;
	a->setCurrent(new Falling());
	a->setCurrentName("falling");
	delete this;
}

void Jumping::climbing(PlayerFSM* a)
{
	std::cout << "Jumping -> Climbing" << std::endl;
	a->setCurrent(new Climbing());
	a->setCurrentName("climbing");
	delete this;
}