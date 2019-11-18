#ifndef CLIMBING_H
#define CLIMBING_H

#include <State.h>

class Climbing : public State
{
public:
	Climbing() {};
	~Climbing() {};

	void handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input);
	void update(PlayerFSM* a, AnimatedSprite* t_animatedSprite);

	void idle(PlayerFSM* a);
	void jumping(PlayerFSM* a);
	void falling(PlayerFSM* a);

private:
	sf::Clock m_clock;
};

#endif // !IDLE_H