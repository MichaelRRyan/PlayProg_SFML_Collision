#include <iostream>
#include <npc.h>
#include <Idle.h>
#include <Debug.h>

NPC::NPC() : GameObject()
{
	m_player_fsm.setCurrent(new Idle());
	m_player_fsm.setPrevious(new Idle());

	m_boundingRect.setOutlineThickness(-4.0f);
	m_boundingRect.setFillColor(sf::Color::Transparent);
	m_boundingRect.setOutlineColor(sf::Color::Green);
}

NPC::NPC(const AnimatedSprite& s) : GameObject(s)
{
	m_player_fsm.setCurrent(new Idle());
	m_player_fsm.setPrevious(new Idle());

	m_boundingRect.setOutlineThickness(-4.0f);
	m_boundingRect.setFillColor(sf::Color::Transparent);
	m_boundingRect.setOutlineColor(sf::Color::Green);
}

NPC::~NPC()
{
}

AnimatedSprite& NPC::getAnimatedSprite()
{
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));
	return m_animated_sprite;
}

sf::RectangleShape& NPC::getBoundingRect()
{
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));

	m_boundingRect.setSize({ m_animated_sprite.getGlobalBounds().width, m_animated_sprite.getGlobalBounds().height });
	m_boundingRect.setPosition(m_animated_sprite.getPosition());

	return m_boundingRect;
}

void NPC::handleInput(Input in)
{
	DEBUG_MSG("Handle Input");

	switch (in.getCurrent())
	{
	case Input::Action::IDLE:
		//std::cout << "Player Idling" << std::endl;
		m_player_fsm.idle();
		break;
	case Input::Action::UP:
		//std::cout << "Player Up" << std::endl;
		m_player_fsm.climbing();
		break;
	case Input::Action::LEFT:
		//std::cout << "Player Left" << std::endl;
		m_player_fsm.jumping();
		break;
	case Input::Action::RIGHT:
		//std::cout << "Player Idling" << std::endl;
		m_player_fsm.jumping();
		break;
	default:
		break;
	}
}

void NPC::update()
{
	//std::cout << "Handle Update" << std::endl;
	m_animated_sprite.update();
}