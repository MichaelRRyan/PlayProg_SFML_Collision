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

using namespace std;

enum class DebugView
{
	Default, // Show only the sprites
	Debug, // Show both bounding rectangle and sprites
	BoundingOnly // Only show bounding rectangles
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

	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.707f, 0.707f);
	float speed{ 0.1f };
	
	DebugView m_debugView{ DebugView::Default };

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
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - playerOffset);
		
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

				if (sf::Keyboard::D == event.key.code)
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
		result = c2AABBtoAABB(aabb_player, aabb_npc);
		if (result){
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

		// Clear screen
		window.clear();

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


		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};