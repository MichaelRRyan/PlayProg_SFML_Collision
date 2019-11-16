#pragma once

#include <SFML/Graphics.hpp>

class RayLine
{
public:
	RayLine(sf::Vector2f t_start, sf::Vector2f t_end);
	void draw(sf::RenderWindow& t_window);
	sf::Vector2f getStartPoint() const;
	sf::Vector2f getDirection() const;
	float getDistance() const;
	void setColor(sf::Color t_color);

private:
	sf::VertexArray m_line{ sf::Lines };
	sf::Vector2f t_points[2];
};

