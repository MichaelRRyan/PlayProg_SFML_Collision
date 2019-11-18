#include <iostream>
#include <Player.h>
#include <Idle.h>
#include <Debug.h>

Player::Player()
{
	// Set up the default player
	m_state.setCurrent(new Idle());
	//m_state.setPrevious(new Idle());
	m_animated_sprite.setOrigin(41.0f, 0.0);
}

Player::Player(const AnimatedSprite& s) : m_animated_sprite(s)
{
	// Set up the default player with an animated sprite
	m_state.setCurrent(new Idle());
	//m_state.setPrevious(new Idle());
	m_animated_sprite.setOrigin(41.0f, 0.0);

	m_boundingRect.setOutlineThickness(-4.0f);
	m_boundingRect.setFillColor(sf::Color::Transparent);
	m_boundingRect.setOutlineColor(sf::Color::Green);
}

Player::~Player() {}

AnimatedSprite& Player::getAnimatedSprite()
{
	// Return the current animating frame
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));
	return m_animated_sprite;
}

sf::RectangleShape& Player::getBoundingRect()
{
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));

	m_boundingRect.setSize({ m_animated_sprite.getGlobalBounds().width, m_animated_sprite.getGlobalBounds().height });
	m_boundingRect.setOrigin(m_boundingRect.getSize().x / 2.0f, 0.0f);
	m_boundingRect.setPosition(m_animated_sprite.getPosition());

	return m_boundingRect;
}

void Player::handleInput(Input t_input)
{
	DEBUG_MSG("Handle Input");

	m_state.handleInput(&m_animated_sprite, t_input); // Handle the input in the states

	// Make sure this code doesn't run more than 60 frames per second
	if (m_clock.getElapsedTime().asSeconds() > 1.0f / 60.0f)
	{
		// Get the name of the current state
		std::string stateName = m_state.getCurrentName();

		// If walking
		if (stateName == "walking")
		{
			if (m_animated_sprite.getScale().x > 0) // Check direction
			{
				m_animated_sprite.move(3.0f, 0.0f); // Move to the right
			}
			else
			{
				m_animated_sprite.move(-3.0f, 0.0f); // Move to the left
			}
			
		}

		// If falling
		if (stateName == "falling")
		{
			m_animated_sprite.move(0.0f, 5.0f); // Move downwards
		}

		// If jumping
		if (stateName == "jumping")
		{
			m_animated_sprite.move(0.0f, -5.0f); // Move upwards
		}

		// If climbing
		if (stateName == "climbing")
		{
			m_animated_sprite.move(0.0f, -3.0f); // Move upwards
		}

		// Check boundary collisions
		// Loop if horizontal
		if (getPositon().x > 800)
		{
			setPosition({ -82.0f, getPositon().y });
		}
		else if (getPositon().x < -82)
		{
			setPosition({ 800, getPositon().y });
		}

		// Block player movement if vertical
		if (getPositon().y > 600 - 82)
		{
			setPosition({ getPositon().x, 600 - 82 });
		}
		else if (getPositon().y < 0)
		{
			setPosition({ getPositon().x, 0 });
		}

		m_clock.restart(); // Restart the clock for next update
	}
}

void Player::update()
{
	m_animated_sprite.update(); // Update the animated sprite
	m_state.update(&m_animated_sprite);
}

void Player::setPosition(sf::Vector2f t_position)
{
	m_animated_sprite.setPosition(t_position); // Set the player position
}

sf::Vector2f Player::getPositon()
{
	return m_animated_sprite.getPosition(); // Get the current player position
}
