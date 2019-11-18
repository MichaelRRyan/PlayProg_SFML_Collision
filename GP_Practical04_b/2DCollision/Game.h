#ifndef GAME_H
#define GAME_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 18/11/2019
/// </summary>

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h> 
#include <npc.h>
#include <Input.h>
#include <Debug.h>
#include <PolygonShape.h>
#include <RayLine.h>

enum class DebugView
{
	Default, // Show only the sprites
	Debug, // Show both bounding rectangle and sprites
	BoundingOnly // Only show bounding rectangles
};

enum class GameState
{
	BouncingRect,
	CollisionTests
};

enum class Shapes
{
	Rectangle,
	Circle,
	Ray
};

struct Capsule
{
	sf::RectangleShape m_body;
	sf::CircleShape m_top;
	sf::CircleShape m_bottom;

	void setup(sf::Vector2f t_bodySize, sf::Vector2f t_bodyPosition)
	{
		// Change these two to change the capsule
		m_body.setSize(t_bodySize);
		m_body.setPosition(t_bodyPosition);

		// Don't touch these
		m_top.setRadius(t_bodySize.x / 2.0f);
		m_top.setPosition(t_bodyPosition.x, t_bodyPosition.y - m_top.getRadius());
		m_bottom.setRadius(t_bodySize.x / 2.0f);
		m_bottom.setPosition(t_bodyPosition.x, t_bodyPosition.y + t_bodySize.y - m_top.getRadius());
	}

	void setColor(sf::Color t_color)
	{
		m_body.setFillColor(t_color);
		m_top.setFillColor(t_color);
		m_bottom.setFillColor(t_color);
	}

	void draw(sf::RenderWindow& t_window)
	{
		t_window.draw(m_body);
		t_window.draw(m_top);
		t_window.draw(m_bottom);
	}
};

class Game
{
public:
	Game();
	void update();

private:
	void setupTextures();
	void setupSprites();
	void setupAABBBounds();
	void setupCollisionObjects();

	// Create the main window
	sf::RenderWindow window;

	sf::Texture npc_texture;
	sf::Texture player_texture;
	sf::Texture m_playerRectTexture;

	AnimatedSprite npc_animated_sprite;
	AnimatedSprite player_animated_sprite;

	// Setup the NPC
	GameObject* npc;

	// Setup the Player
	GameObject* player;

	sf::Vector2f playerOffset;
	c2AABB aabb_npc;
	c2AABB aabb_player;

	Capsule visualCapsule;
	c2Capsule collisionCapsule;

	PolygonShape polygonShape;

	c2Poly collisionPolygon;

	RayLine ray;

	c2Raycast raycast;
	c2Ray collisionRay;

	sf::CircleShape circleShape;
	c2Circle collisionCircle;
	sf::CircleShape stationaryCircleShape;
	c2Circle stationaryCollisionCircle;

	// Initialize Input
	Input input;

	// Direction of movement of NPC
	sf::Vector2f m_direction;
	float speed{ 0.1f };

	DebugView m_debugView{ DebugView::Default };
	GameState m_gameState{ GameState::BouncingRect };
	Shapes m_selectedShape{ Shapes::Rectangle };

	sf::Font font;
	sf::Text debugText;

	bool upButtonDown;
};

#endif // !GAME_H