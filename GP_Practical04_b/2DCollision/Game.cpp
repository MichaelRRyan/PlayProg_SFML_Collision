#include "Game.h"

Game::Game() :
	window(sf::VideoMode(800, 600), "SFML window"),
	npc_animated_sprite(npc_texture),
	player_animated_sprite(player_texture),
	ray({ 500.0f, 100.0f }, { 600.0f, 200.0f }),
	m_direction(0.707f, 0.707f),
	polygonShape{ {300.0f, 100.0f}, {100.0f, 100.0f} }
{
	setupTextures();
	setupSprites();

	npc = new NPC(npc_animated_sprite);
	player = new Player(player_animated_sprite);
	playerOffset = { 0.0f, player->getAnimatedSprite().getGlobalBounds().height / 2.0f };

	setupAABBBounds();
	setupCollisionObjects();
}

void Game::setupTextures()
{
	// Load a NPC's sprites to display
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
	}

	// Load a mouse texture to display
	if (!m_playerRectTexture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
	}

	// Load a sprite to display
	if (!player_texture.loadFromFile("assets\\sprites.png")) {
		DEBUG_MSG("Failed to load file");
	}
}

void Game::setupSprites()
{
	// Setup NPC's Default Animated Sprite
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Add all the textures
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			player_animated_sprite.addFrame(sf::IntRect(3 + (85 * col), 3 + (85 * row), 82, 82));
		}
	}
}

void Game::setupAABBBounds()
{
	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc->getAnimatedSprite().getPosition().x, npc->getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc->getAnimatedSprite().getPosition().x +
		npc->getAnimatedSprite().getGlobalBounds().width,
		npc->getAnimatedSprite().getPosition().y +
		npc->getAnimatedSprite().getGlobalBounds().height);

	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player->getAnimatedSprite().getPosition().x, player->getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player->getAnimatedSprite().getGlobalBounds().width / 6, player->getAnimatedSprite().getGlobalBounds().width / 6);
}

void Game::setupCollisionObjects()
{
	////////////////////////////////////////////////////////////////////////////////
	/// CAPSULE
	////////////////////////////////////////////////////////////////////////////////
	visualCapsule.setup({ 80.0f, 50.0f }, { 100.0f, 100.0f });

	collisionCapsule.a.x = visualCapsule.m_body.getPosition().x + visualCapsule.m_top.getRadius();
	collisionCapsule.a.y = visualCapsule.m_body.getPosition().y;

	collisionCapsule.b.x = visualCapsule.m_body.getPosition().x + visualCapsule.m_top.getRadius();
	collisionCapsule.b.y = visualCapsule.m_body.getPosition().y + visualCapsule.m_body.getSize().y;

	collisionCapsule.r = visualCapsule.m_top.getRadius();

	////////////////////////////////////////////////////////////////////////////////
	/// Polygon
	////////////////////////////////////////////////////////////////////////////////

	collisionPolygon.count = 3;

	for (int i = 0; i < 3; i++)
	{
		collisionPolygon.verts[i] = { polygonShape.getPointPosition(i).x, polygonShape.getPointPosition(i).y };
	}
	c2MakePoly(&collisionPolygon);

	////////////////////////////////////////////////////////////////////////////////
	/// Ray
	////////////////////////////////////////////////////////////////////////////////
	collisionRay.p = { ray.getStartPoint().x, ray.getStartPoint().y };
	collisionRay.d = { ray.getDirection().x, ray.getDirection().y };
	collisionRay.t = ray.getDistance();

	////////////////////////////////////////////////////////////////////////////////
	/// Circle
	////////////////////////////////////////////////////////////////////////////////
	circleShape.setRadius(40);
	circleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
	circleShape.setOutlineThickness(-4.0f);
	circleShape.setOutlineColor(sf::Color::Green);
	circleShape.setFillColor(sf::Color::Transparent);

	collisionCircle.r = circleShape.getRadius();

	stationaryCircleShape.setRadius(40);
	stationaryCircleShape.setOrigin(circleShape.getRadius(), circleShape.getRadius());
	stationaryCircleShape.setPosition(500.0f, 440.0f);

	stationaryCollisionCircle.r = stationaryCircleShape.getRadius();
	stationaryCollisionCircle.p = { stationaryCircleShape.getPosition().x, stationaryCircleShape.getPosition().y };

	if (!font.loadFromFile("assets/arial.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	debugText.setFont(font);
	debugText.setString("Press M to switch between game states\nPress D to toggle through debug modes");
	debugText.setCharacterSize(25);
}

void Game::update()
{
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		if (m_gameState == GameState::CollisionTests)
		{
			if (m_selectedShape == Shapes::Rectangle)
			{
				player->getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) - playerOffset);

				// Update Player AABB
				aabb_player.min = c2V(
					player->getAnimatedSprite().getPosition().x - player->getAnimatedSprite().getGlobalBounds().width / 2.0f,
					player->getAnimatedSprite().getPosition().y
				);
				aabb_player.max = c2V(
					player->getAnimatedSprite().getPosition().x +
					player->getAnimatedSprite().getGlobalBounds().width / 2.0f,
					player->getAnimatedSprite().getPosition().y +
					player->getAnimatedSprite().getGlobalBounds().height
				);
			}
			else if (m_selectedShape == Shapes::Circle)
			{
				circleShape.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				collisionCircle.p = { circleShape.getPosition().x, circleShape.getPosition().y };
			}
			else if (m_selectedShape == Shapes::Ray)
			{
				ray.setEndPoint(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				collisionRay.p = { ray.getStartPoint().x, ray.getStartPoint().y };
				collisionRay.d = { ray.getDirection().x, ray.getDirection().y };
				collisionRay.t = ray.getDistance();
			}
		}
		else
		{
			// Update Player AABB
			aabb_player.min = c2V(
				player->getAnimatedSprite().getPosition().x - player->getAnimatedSprite().getGlobalBounds().width / 2.0f,
				player->getAnimatedSprite().getPosition().y
			);
			aabb_player.max = c2V(
				player->getAnimatedSprite().getPosition().x +
				player->getAnimatedSprite().getGlobalBounds().width / 2.0f,
				player->getAnimatedSprite().getPosition().y +
				player->getAnimatedSprite().getGlobalBounds().height
			);

			// Move The NPC
			sf::Sprite& npcSprite = npc->getAnimatedSprite();

			if (npcSprite.getPosition().x < 0) {
				m_direction.x *= -1;
				npcSprite.setPosition(0.0f, npcSprite.getPosition().y);
			}
			else if (npcSprite.getPosition().x + npc->getAnimatedSprite().getGlobalBounds().width >= 800) {
				m_direction.x *= -1;
				npcSprite.setPosition(800 - npcSprite.getGlobalBounds().width, npcSprite.getPosition().y);
			}
			else if (npcSprite.getPosition().y < 0) {
				m_direction.y *= -1;
				npcSprite.setPosition(npcSprite.getPosition().x, 0.0f);
			}
			else if (npcSprite.getPosition().y + npc->getAnimatedSprite().getGlobalBounds().height >= 600) {
				m_direction.y *= -1;
				npcSprite.setPosition(npcSprite.getPosition().x, 600 - npcSprite.getGlobalBounds().height);
			}

			npc->getAnimatedSprite().move(m_direction * speed);

			// Update NPC AABB set x and y
			aabb_npc.min = c2V(
				npc->getAnimatedSprite().getPosition().x,
				npc->getAnimatedSprite().getPosition().y
			);

			aabb_npc.max = c2V(
				npc->getAnimatedSprite().getPosition().x +
				npc->getAnimatedSprite().getGlobalBounds().width,
				npc->getAnimatedSprite().getPosition().y +
				npc->getAnimatedSprite().getGlobalBounds().height
			);
		}

		input.setAllFalse();

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
				if (sf::Keyboard::Up == event.key.code)
				{
					if (!upButtonDown) // Check if the button was not already down
					{
						input.m_upPressed = true; // Sets the up button to true
						upButtonDown = true; // Sets the up button to down position
					}
				}
				if (sf::Keyboard::Space == event.key.code)
				{
					input.m_spacePressed = true;
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
						debugText.setString("Press M to switch between game states\nPress S to change selected shape");
						break;
					case GameState::CollisionTests:
						m_gameState = GameState::BouncingRect;
						player->getBoundingRect().setOutlineThickness(-4.0f);
						player->getBoundingRect().setFillColor(sf::Color::Transparent);
						debugText.setString("Press M to switch between game states\nPress D to toggle through debug modes");
						break;
					}

					m_selectedShape = Shapes::Rectangle;
					m_debugView = DebugView::Default;
					player->getBoundingRect().setOutlineColor(sf::Color::Green);
				}
				if (sf::Keyboard::S == event.key.code
					&& m_gameState == GameState::CollisionTests)
				{
					switch (m_selectedShape)
					{
					case Shapes::Rectangle:
						m_selectedShape = Shapes::Circle;
						player->getBoundingRect().setOutlineThickness(0.0f);
						player->getAnimatedSprite().setPosition(200.0f, 400.0f);

						// Update Player AABB
						aabb_player.min = c2V(
							player->getAnimatedSprite().getPosition().x,
							player->getAnimatedSprite().getPosition().y
						);
						aabb_player.max = c2V(
							player->getAnimatedSprite().getPosition().x +
							player->getAnimatedSprite().getGlobalBounds().width,
							player->getAnimatedSprite().getPosition().y +
							player->getAnimatedSprite().getGlobalBounds().height
						);

						break;
					case Shapes::Circle:
						m_selectedShape = Shapes::Ray;
						ray.setStartPoint({ 400.0f, 300.0f });
						ray.setColor(sf::Color::Green);
						break;
					case Shapes::Ray:
						m_selectedShape = Shapes::Rectangle;
						player->getBoundingRect().setOutlineThickness(-4.0f);
						player->getBoundingRect().setFillColor(sf::Color::Transparent);

						ray.setColor(sf::Color::White);
						ray.setStartPoint({ 500.0f, 100.0f });
						ray.setEndPoint({ 600.0f, 200.0f });
						collisionRay.p = { ray.getStartPoint().x, ray.getStartPoint().y };
						collisionRay.d = { ray.getDirection().x, ray.getDirection().y };
						collisionRay.t = ray.getDistance();
						break;
					}
				}
				break;
			case sf::Event::KeyReleased:
				if (sf::Keyboard::Up == event.key.code)
				{
					upButtonDown = false;
				}
				break;
			}
		}

		// Gets the continuous keypress updates
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			input.m_left = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			input.m_right = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			input.m_up = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			input.m_down = true;
		}

		// Handle input to Player
		player->handleInput(input);

		// Update the Player
		player->update();

		// Update the Player
		npc->update();

		// Check for collisions
		if (m_gameState == GameState::CollisionTests)
		{
			if (m_selectedShape == Shapes::Rectangle)
			{
				// Check for AABB to Capsule collisions
				if (c2AABBtoCapsule(aabb_player, collisionCapsule))
				{
					visualCapsule.setColor(sf::Color(255, 0, 0));
				}
				else
				{
					visualCapsule.setColor(sf::Color(255, 255, 255));
				}

				// Check for AABB to Triangle collisions
				if (c2AABBtoPoly(aabb_player, &collisionPolygon, NULL))
				{
					polygonShape.setColor(sf::Color::Red);
				}
				else
				{
					polygonShape.setColor(sf::Color::White);
				}

				// Check for AABB to Ray collisions
				if (c2CastRay(collisionRay, &aabb_player, 0, C2_AABB, &raycast))
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
					player->getBoundingRect().setFillColor(sf::Color::Red);
				}
				else
				{
					player->getBoundingRect().setFillColor(sf::Color::White);
				}

				// Check for Circle to Ray collisions
				if (c2CastRay(collisionRay, &collisionCircle, 0, C2_CIRCLE, &raycast))
				{
					ray.setColor(sf::Color::Red);
				}
				else
				{
					ray.setColor(sf::Color::White);
				}

				// Check for AABB to Capsule collisions
				if (c2CircletoCapsule(collisionCircle, collisionCapsule))
				{
					visualCapsule.setColor(sf::Color(255, 0, 0));
				}
				else
				{
					visualCapsule.setColor(sf::Color(255, 255, 255));
				}

				// Check for AABB to Triangle collisions
				if (c2CircletoPoly(collisionCircle, &collisionPolygon, NULL))
				{
					polygonShape.setColor(sf::Color::Red);
				}
				else
				{
					polygonShape.setColor(sf::Color::White);
				}

				// Check for AABB to Triangle collisions
				if (c2CircletoCircle(collisionCircle, stationaryCollisionCircle))
				{
					stationaryCircleShape.setFillColor(sf::Color::Red);
				}
				else
				{
					stationaryCircleShape.setFillColor(sf::Color::White);
				}
			}
			else if (m_selectedShape == Shapes::Ray)
			{
				// Check for Ray to AABB collisions
				if (c2RaytoAABB(collisionRay, aabb_player, &raycast))
				{
					player->getBoundingRect().setFillColor(sf::Color::Red);
				}
				else
				{
					player->getBoundingRect().setFillColor(sf::Color::White);
				}

				// Check for Ray to Capsule collisions
				if (c2RaytoCapsule(collisionRay, collisionCapsule, &raycast))
				{
					visualCapsule.setColor(sf::Color(255, 0, 0));
				}
				else
				{
					visualCapsule.setColor(sf::Color(255, 255, 255));
				}

				// Check for Ray to Circle collisions
				if (c2RaytoCircle(collisionRay, stationaryCollisionCircle, &raycast))
				{
					stationaryCircleShape.setFillColor(sf::Color::Red);
				}
				else
				{
					stationaryCircleShape.setFillColor(sf::Color::White);
				}

				// Check for Ray to Polygon collisions
				if (c2RaytoPoly(collisionRay, &collisionPolygon, &c2xIdentity(), &raycast))
				{
					polygonShape.setColor(sf::Color::Red);
				}
				else
				{
					polygonShape.setColor(sf::Color::White);
				}
			}
		}
		else if (m_gameState == GameState::BouncingRect)
		{
			// Check for AABB to AABB collisions
			if (c2AABBtoAABB(aabb_player, aabb_npc)) {
				//player.getAnimatedSprite().setColor(sf::Color(255,0,0));
				player->getBoundingRect().setOutlineColor(sf::Color(255, 0, 0));
				npc->getBoundingRect().setOutlineColor(sf::Color(255, 0, 0));
				//cout << "Collision" << endl;
			}
			else {
				//player.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
				player->getBoundingRect().setOutlineColor(sf::Color(0, 255, 0));
				npc->getBoundingRect().setOutlineColor(sf::Color(0, 255, 0));
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
			window.draw(player->getBoundingRect());

			if (m_selectedShape == Shapes::Circle)
			{
				window.draw(stationaryCircleShape);
				window.draw(circleShape);
			}
			if (m_selectedShape == Shapes::Ray)
			{
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
				window.draw(player->getAnimatedSprite());

				// Draw the NPC's Current Animated Sprite
				window.draw(npc->getAnimatedSprite());
			}

#ifdef _DEBUG
			if (m_debugView == DebugView::BoundingOnly
				|| m_debugView == DebugView::Debug)
			{
				// Draw the Player's Current Bounding Rect
				window.draw(player->getBoundingRect());

				// Draw the NPC's Current Bounding Rect
				window.draw(npc->getBoundingRect());
			}


#endif // _DEBUG
		}

		window.draw(debugText);

		// Update the window
		window.display();
	}
}
