#ifndef NPC_H
#define NPC_H

#include <GameObject.h>
#include <Input.h>
#include <PlayerFSM.h>
#include <AnimatedSprite.h>

class NPC : public GameObject
{

public:
	NPC();
	~NPC();
	NPC(const AnimatedSprite&);
	AnimatedSprite& getAnimatedSprite();
	sf::RectangleShape &getBoundingRect();
	void handleInput(Input);
	void update();
};

#endif // !NPC_H
