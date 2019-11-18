#include <Climbing.h>
#include <Jumping.h>
#include <Idle.h>
#include <Falling.h>

#include <string>

void Climbing::handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input)
{
	// Check the input and move to the appropriate state
	if (t_input.m_spacePressed)
	{
		t_animatedSprite->changeAnimation(Animation::Jumping);
		jumping(a);
	}
	else if (!t_input.m_up) // if no longer holding climbing button, just fall
	{
		t_animatedSprite->changeAnimation(Animation::Falling);
		falling(a);
	}
}

void Climbing::update(PlayerFSM* a, AnimatedSprite* t_animatedSprite)
{
	if (m_clock.getElapsedTime().asSeconds() > 1.0f) // Check if the animation time has elapsed, change to new animation
	{
		t_animatedSprite->changeAnimation(Animation::Idle);
		idle(a);
	}
}

void Climbing::idle(PlayerFSM* a)
{
	std::cout << "Climbing -> Idle" << std::endl;
	a->setCurrent(new Idle());
	a->setCurrentName("idle");
	delete this;
}
void Climbing::jumping(PlayerFSM* a)
{
	std::cout << "Climbing -> Jump" << std::endl;
	a->setCurrent(new Jumping());
	a->setCurrentName("jumping");
	delete this;
}

void Climbing::falling(PlayerFSM* a)
{
	std::cout << "Climbing -> Falling" << std::endl;
	a->setCurrent(new Falling());
	a->setCurrentName("falling");
	delete this;
}
