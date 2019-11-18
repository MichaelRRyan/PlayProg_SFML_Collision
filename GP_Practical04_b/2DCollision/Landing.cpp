#include "Landing.h"
#include "Idle.h"

#include <string>

Landing::Landing()
{
	m_clock.restart();
}

void Landing::update(PlayerFSM* a, AnimatedSprite* t_animatedSprite)
{
	if (m_clock.getElapsedTime().asSeconds() > 1.0f) // Check if one second has elapsed since started landing
	{
		t_animatedSprite->changeAnimation(Animation::Idle);
		idle(a);
	}
}

void Landing::idle(PlayerFSM* a)
{
	std::cout << "Landing -> Idle" << std::endl;
	a->setCurrent(new Idle());
	a->setCurrentName("idle");
	delete this;
}
