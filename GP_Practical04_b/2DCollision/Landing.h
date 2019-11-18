#ifndef LANDING_H
#define LANDING_H

#include "State.h"

class Landing : public State
{
public:
	Landing();
	~Landing() {};

	void update(PlayerFSM* a, AnimatedSprite* t_animatedSprite);

	void idle(PlayerFSM *a);

private:
	sf::Clock m_clock;
};

#endif // !LANDING_H




