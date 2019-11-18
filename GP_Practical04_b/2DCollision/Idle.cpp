#include <Idle.h>
#include <Jumping.h>
#include <Climbing.h>
#include "Walking.h"
#include "Falling.h"

#include <string>

void Idle::handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input)
{
	// Check input, move to the corresponding state
	if (t_input.m_spacePressed)
	{
		t_animatedSprite->changeAnimation(Animation::Jumping);
		jumping(a);
	}
	else if (t_input.m_upPressed)
	{
		t_animatedSprite->changeAnimation(Animation::Climbing);
		climbing(a);
	}
	else if (t_input.m_right)
	{
		t_animatedSprite->setScale(1.0f, 1.0f);
		t_animatedSprite->changeAnimation(Animation::Walking);
		walking(a);
	}
	else if (t_input.m_left)
	{
		t_animatedSprite->setScale(-1.0f, 1.0f);
		t_animatedSprite->changeAnimation(Animation::Walking);
		walking(a);
	}
	else if (t_input.m_down)
	{
		t_animatedSprite->changeAnimation(Animation::Falling);
		falling(a);
	}
}

void Idle::update(PlayerFSM* a, AnimatedSprite* t_animatedSprite)
{
}

void Idle::jumping(PlayerFSM* a)
{
	std::cout << "Idle -> Jumping" << std::endl;
	a->setCurrent(new Jumping());
	a->setCurrentName("jumping");
	delete this;
}
void Idle::climbing(PlayerFSM* a)
{
	std::cout << "Idle -> Climbing" << std::endl;
	a->setCurrent(new Climbing());
	a->setCurrentName("climbing");
	delete this;
}

void Idle::walking(PlayerFSM* a)
{
	std::cout << "Idle -> Walking" << std::endl;
	a->setCurrent(new Walking());
	a->setCurrentName("walking");
	delete this;
}

void Idle::falling(PlayerFSM* a)
{
	std::cout << "Idle -> Falling" << std::endl;
	a->setCurrent(new Falling());
	a->setCurrentName("falling");
	delete this;
}
