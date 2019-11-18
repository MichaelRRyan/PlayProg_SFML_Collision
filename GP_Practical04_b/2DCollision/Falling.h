#ifndef FALLING_H
#define FALLING_H

#include <State.h>

class Falling : public State
{
public:
	Falling();
	~Falling() {};

	void update(PlayerFSM* a, AnimatedSprite* t_animatedSprite);

	void landing(PlayerFSM* a);

private:
	sf::Clock m_clock;
};

#endif // !FALLING_H