#ifndef WALKING_H
#define WALKING_H

#include "State.h"

class Walking : public State
{
public:
	Walking() {};
	~Walking() {};

	void handleInput(PlayerFSM* a, AnimatedSprite* t_animatedSprite, Input t_input);
	void update(PlayerFSM* a, AnimatedSprite* t_animatedSprite);

	void jumping(PlayerFSM* a);
	void idle(PlayerFSM* a);
	void falling(PlayerFSM* a);
};

#endif // !WALKING_H