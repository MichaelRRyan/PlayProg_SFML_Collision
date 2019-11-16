#include "PolygonShape.h"

PolygonShape::PolygonShape(sf::Vector2f t_position, sf::Vector2f t_size) :
	m_pointPositions{
		{t_position.x + (t_size.x / 2.0f), t_position.y},
		{t_position.x + t_size.x, t_position.y + t_size.y},
		{t_position.x, t_position.y + t_size.y}
}
{
	m_shapePoints.append({ {m_pointPositions[0]}, sf::Color::White });
	m_shapePoints.append({ {m_pointPositions[1]}, sf::Color::White });
	m_shapePoints.append({ {m_pointPositions[2]}, sf::Color::White });
}

void PolygonShape::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_shapePoints);
}

sf::Vector2f PolygonShape::getPointPosition(int t_pointIndex)
{
	return m_pointPositions[t_pointIndex];
}

void PolygonShape::setColor(sf::Color t_color)
{
	m_shapePoints.clear();
	for (int i = 0; i < 3; i++)
	{
		m_shapePoints.append({ m_pointPositions[i], t_color });
	}
}
