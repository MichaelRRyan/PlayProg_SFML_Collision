#ifndef PLAYER_H
#define PLAYER_H

#include <GameObject.h>
#include <Input.h>
#include <PlayerFSM.h>
#include <AnimatedSprite.h>

class Player : public GameObject
{
private:
	PlayerFSM m_state;
	AnimatedSprite m_animated_sprite;
	sf::Clock m_clock;
	Player();

public:
	Player(const AnimatedSprite&);
	~Player();
	AnimatedSprite& getAnimatedSprite();
	sf::RectangleShape& getBoundingRect();
	void handleInput(Input t_input);
	void update();
	void setPosition(sf::Vector2f t_position);
	sf::Vector2f getPositon();
};

#endif // !PLAYER_H
