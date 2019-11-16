#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>
#include <PolygonShape.h>
#include <RayLine.h>

using namespace std;

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
	Circle
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

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup the NPC
	GameObject &npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject &player = Player(player_animated_sprite);

	sf::Vector2f playerOffset{ player.getAnimatedSprite().getGlobalBounds().width / 2.0f, player.getAnimatedSprite().getGlobalBounds().height / 2.0f };

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width, 
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);

	////////////////////////////////////////////////////////////////////////////////
	/// CAPSULE
	////////////////////////////////////////////////////////////////////////////////
	Capsule visualCapsule;
	visualCapsule.setup({ 80.0f, 50.0f }, { 100.0f, 100.0f });

	c2Capsule collisionCapsule;
	collisionCapsule.a.x = visualCapsule.m_body.getPosition().x + visualCapsule.m_top.getRadius();
	collisionCapsule.a.y = visualCapsule.m_body.getPosition().y;

	collisionCapsule.b.x = visualCapsule.m_body.getPosition().x + visualCapsule.m_top.getRadius();
	collisionCapsule.b.y = visualCapsule.m_body.getPosition().y + visualCapsule.m_body.getSize().y;

	collisionCapsule.r = visualCapsule.m_top.getRadius();

	////////////////////////////////////////////////////////////////////////////////
	/// Polygon
	////////////////////////////////////////////////////////////////////////////////
	PolygonShape polygonShape{ {300.0f, 100.0f}, {100.0f, 100.0f} };

	c2Poly collisionPolygon;
	collisionPolygon.count = 3;

	for (int i = 0; i < 3; i++)
	{
		collisionPolygon.verts[i] = { polygonShape.getPointPosition(i).x, polygonShape.getPointPosition(i).y };
	}

	////////////////////////////////////////////////////////////////////////////////
	/// Ray
	////////////////////////////////////////////////////////////////////////////////
	RayLine ray({ 500.0f, 100.0f }, { 600.0f, 200.0f });

	c2Ray collisionRay;
	collisionRay.p = { ray.getStartPoint().x, ray.getStartPoint().y };
	collisionRay.d = { ray.getDirection().x, ray.getDirection().y };
	collisionRay.t = ray.getDistance();

	c2Raycast raycast;
	raycast.n = { 0, 0 };
	raycast.t = 0;

	////////////////////////////////////////////////////////////////////////////////
	/// Circle
	////////////////////////////////////////////////////////////////////////////////
	sf::CircleShape circleShape;
	circleShape.setRadius(40);
	circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
	circleShape.setOutlineThickness(-4.0f);
	circleShape.setOutlineColor(sf::Color::Green);
	circleShape.setFillColor(sf::Color::Transparent);

	c2Circle collisionCircle;
	collisionCircle.r = circleShape.getRadius();
	

	sf::CircleShape stationaryCircleShape;
	stationaryCircleShape.setRadius(40);
	stationaryCircleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
	stationaryCircleShape.setPosition(500.0f, 440.0f);

	c2Circle stationaryCollisionCircle;
	stationaryCollisionCircle.r = stationaryCircleShape.getRadius();
	stationaryCollisionCircle.p = { stationaryCircleShape.getPosition().x, stationaryCircleShape.getPosition().y };

	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.707f, 0.707f);
	float speed{ 0.1f };
	
	DebugView m_debugView{ DebugView::Default };
	GameState m_gameState{ GameState::BouncingRect };
	Shapes m_selectedShape{ Shapes::Rectangle };

	sf::Font font;
	sf::Text debugText;

	if (!font.loadFromFile("assets/arial.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	debugText.setFont(font);
	debugText.setString("Press D to toggle through debug modes");
	debugText.setCharacterSize(25);

	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		if (m_selectedShape == Shapes::Rectangle
			|| m_gameState == GameState::BouncingRect)
		{
			player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - playerOffset);

			// Update Player AABB
			aabb_player.min = c2V(
				player.getAnimatedSprite().getPosition().x,
				player.getAnimatedSprite().getPosition().y
			);
			aabb_player.max = c2V(
				player.getAnimatedSprite().getPosition().x +
				player.getAnimatedSprite().getGlobalBounds().width,
				player.getAnimatedSprite().getPosition().y +
				player.getAnimatedSprite().getGlobalBounds().height
			);
		}
		else if (m_selectedShape == Shapes::Circle)
		{
			circleShape.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			collisionCircle.p = { circleShape.getPosition().x, circleShape.getPosition().y };
		}
		
		
		// Move The NPC
		sf::Sprite& npcSprite = npc.getAnimatedSprite();

		if (npcSprite.getPosition().x < 0) {
			direction.x *= -1;
			npcSprite.setPosition(0.0f, npcSprite.getPosition().y);
		}
		else if (npcSprite.getPosition().x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) {
			direction.x *= -1;
			npcSprite.setPosition(800 - npcSprite.getGlobalBounds().width, npcSprite.getPosition().y);
		}
		else if (npcSprite.getPosition().y < 0) {
			direction.y *= -1;
			npcSprite.setPosition(npcSprite.getPosition().x, 0.0f);
		}
		else if (npcSprite.getPosition().y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			npcSprite.setPosition(npcSprite.getPosition().x, 600 - npcSprite.getGlobalBounds().height);
		}
		
		npc.getAnimatedSprite().move(direction * speed);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
				}

				if (sf::Keyboard::D == event.key.code
					&& m_gameState == GameState::BouncingRect)
				{
					switch (m_debugView)
					{
					case DebugView::Default:
						m_debugView = DebugView::Debug;
						break;
					case DebugView::Debug:
						m_debugView = DebugView::BoundingOnly;
						break;
					case DebugView::BoundingOnly:
						m_debugView = DebugView::Default;
						break;
					}
				}
				if (sf::Keyboard::M == event.key.code)
				{
					switch (m_gameState)
					{
					case GameState::BouncingRect:
						m_gameState = GameState::CollisionTests;
						break;
					case GameState::CollisionTests:
						m_gameState = GameState::BouncingRect;
						player.getBoundingRect().setOutlineThickness(-4.0f);
						player.getBoundingRect().setFillColor(sf::Color::Transparent);
						break;
					}

					m_selectedShape = Shapes::Rectangle;
					m_debugView = DebugView::Default;
					player.getBoundingRect().setOutlineColor(sf::Color::Green);
				}
				if (sf::Keyboard::S == event.key.code
					&& m_gameState == GameState::CollisionTests)
				{
					switch (m_selectedShape)
					{
					case Shapes::Rectangle:
						m_selectedShape = Shapes::Circle;
						player.getBoundingRect().setOutlineThickness(0.0f);
						player.getAnimatedSprite().setPosition(200.0f, 400.0f);

						// Update Player AABB
						aabb_player.min = c2V(
							player.getAnimatedSprite().getPosition().x,
							player.getAnimatedSprite().getPosition().y
						);
						aabb_player.max = c2V(
							player.getAnimatedSprite().getPosition().x +
							player.getAnimatedSprite().getGlobalBounds().width,
							player.getAnimatedSprite().getPosition().y +
							player.getAnimatedSprite().getGlobalBounds().height
						);

						break;
					case Shapes::Circle:
						m_selectedShape = Shapes::Rectangle;
						player.getBoundingRect().setOutlineThickness(-4.0f);
						player.getBoundingRect().setFillColor(sf::Color::Transparent);
						break;
					}
				}
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

		// Check for collisions
		if (m_gameState == GameState::CollisionTests)
		{
			if (m_selectedShape == Shapes::Rectangle)
			{
				// Check for AABB to Capsule collisions
				result = c2AABBtoCapsule(aabb_player, collisionCapsule);
				if (result)
				{
					visualCapsule.setColor(sf::Color(255, 0, 0));
				}
				else
				{
					visualCapsule.setColor(sf::Color(255, 255, 255));
				}

				// Check for AABB to Triangle collisions
				result = c2AABBtoPoly(aabb_player, &collisionPolygon, NULL);
				if (result)
				{
					polygonShape.setColor(sf::Color::Red);
				}
				else
				{
					polygonShape.setColor(sf::Color::White);
				}

				// Check for AABB to Ray collisions
				result = c2CastRay(collisionRay, &aabb_player, 0, C2_AABB, &raycast);
				if (result)
				{
					ray.setColor(sf::Color::Red);
				}
				else
				{
					ray.setColor(sf::Color::White);
				}
			}
			else if (m_selectedShape == Shapes::Circle)
			{
				// Check for Circle to AABB collisions
				if (c2CircletoAABB(collisionCircle, aabb_player))
				{
					player.getBoundingRect().setFillColor(sf::Color::Red);
				}
				else
				{
					player.getBoundingRect().setFillColor(sf::Color::White);
				}

				// Check for Circle to Ray collisions
				result = c2CastRay(collisionRay, &collisionCircle, 0, C2_CIRCLE, &raycast);
				if (result)
				{
					ray.setColor(sf::Color::Red);
				}
				else
				{
					ray.setColor(sf::Color::White);
				}

				// Check for AABB to Capsule collisions
				result = c2CircletoCapsule(collisionCircle, collisionCapsule);
				if (result)
				{
					visualCapsule.setColor(sf::Color(255, 0, 0));
				}
				else
				{
					visualCapsule.setColor(sf::Color(255, 255, 255));
				}

				// Check for AABB to Triangle collisions
				result = c2CircletoPoly(collisionCircle, &collisionPolygon, NULL);
				if (result)
				{
					polygonShape.setColor(sf::Color::Red);
				}
				else
				{
					polygonShape.setColor(sf::Color::White);
				}

				// Check for AABB to Triangle collisions
				result = c2CircletoCircle(collisionCircle, stationaryCollisionCircle);
				if (result)
				{
					stationaryCircleShape.setFillColor(sf::Color::Red);
				}
				else
				{
					stationaryCircleShape.setFillColor(sf::Color::White);
				}
			}
		}
		else if (m_gameState == GameState::BouncingRect)
		{
			// Check for AABB to AABB collisions
			result = c2AABBtoAABB(aabb_player, aabb_npc);
			if (result) {
				//player.getAnimatedSprite().setColor(sf::Color(255,0,0));
				player.getBoundingRect().setOutlineColor(sf::Color(255, 0, 0));
				npc.getBoundingRect().setOutlineColor(sf::Color(255, 0, 0));
				//cout << "Collision" << endl;
			}
			else {
				//player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
				player.getBoundingRect().setOutlineColor(sf::Color(0, 255, 0));
				npc.getBoundingRect().setOutlineColor(sf::Color(0, 255, 0));
			}
		}

		// Clear screen
		window.clear();

		if (m_gameState == GameState::CollisionTests)
		{
			// Draw shapes
			visualCapsule.draw(window);
			polygonShape.draw(window);
			ray.draw(window);
			window.draw(player.getBoundingRect());

			if (m_selectedShape == Shapes::Circle)
			{
				window.draw(circleShape);
				window.draw(stationaryCircleShape);
			}
		}
		else if (m_gameState == GameState::BouncingRect)
		{
#ifdef _DEBUG
			if (m_debugView != DebugView::BoundingOnly)
#endif // _DEBUG
			{
				// Draw the Players Current Animated Sprite
				window.draw(player.getAnimatedSprite());

				// Draw the NPC's Current Animated Sprite
				window.draw(npc.getAnimatedSprite());
			}

#ifdef _DEBUG
			if (m_debugView == DebugView::BoundingOnly
				|| m_debugView == DebugView::Debug)
			{
				// Draw the Player's Current Bounding Rect
				window.draw(player.getBoundingRect());

				// Draw the NPC's Current Bounding Rect
				window.draw(npc.getBoundingRect());
			}

			window.draw(debugText);
#endif // _DEBUG
		}


		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};