#pragma once

#include <SFML/Graphics.hpp>

class PolygonShape
{
public:
	PolygonShape(sf::Vector2f t_position, sf::Vector2f t_size);
	void draw(sf::RenderWindow& t_window);
	sf::Vector2f getPointPosition(int t_pointIndex);
	void setColor(sf::Color t_color);

private:
	sf::VertexArray m_shapePoints{ sf::Triangles, 3 };
	sf::Vector2f m_pointPositions[3];
};

