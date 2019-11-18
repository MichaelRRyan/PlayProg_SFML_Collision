#include "Walking.h"
#include "Jumping.h"
#include "Idle.h"
#include "Falling.h"

void Walking::handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input)
{
	// Check input
	if ((t_animatedSprite->getScale().x > 0 && !t_input.m_right) // Check if moving right
		|| (t_animatedSprite->getScale().x < 0 && !t_input.m_left)) // Check if moving left
	{
		t_animatedSprite->changeAnimation(Animation::Idle); // If not moving anymore go back to idle
		idle(a);
	}
	else if (t_input.m_spacePressed)
	{
		t_animatedSprite->changeAnimation(Animation::Jumping);
		jumping(a);
	}
	else if (t_input.m_down)
	{
		t_animatedSprite->changeAnimation(Animation::Falling);
		falling(a);
	}
}

void Walking::update(PlayerFSM* a, AnimatedSprite* t_animatedSprite)
{
}

void Walking::jumping(PlayerFSM* a)
{
	std::cout << "Walking -> Jumping" << std::endl;
	a->setCurrent(new Jumping());
	a->setCurrentName("jumping");
	delete this;
}

void Walking::idle(PlayerFSM* a)
{
	std::cout << "Walking -> Idle" << std::endl;
	a->setCurrent(new Idle());
	a->setCurrentName("idle");
	delete this;
}

void Walking::falling(PlayerFSM* a)
{
	std::cout << "Walking -> Falling" << std::endl;
	a->setCurrent(new Falling());
	a->setCurrentName("falling");
	delete this;
}
