#include "RayLine.h"

RayLine::RayLine(sf::Vector2f t_start, sf::Vector2f t_end) :
	t_points{ t_start, t_end }
{
	m_line.append({ t_start, sf::Color::White });
	m_line.append({ t_end, sf::Color::White });
}

void RayLine::draw(sf::RenderWindow& t_window)
{
	t_window.draw(m_line);
}

sf::Vector2f RayLine::getStartPoint() const
{
	return t_points[0];
}

sf::Vector2f RayLine::getDirection() const
{
	const sf::Vector2f distanceVector = t_points[1] - t_points[0];
	const float magnitude = sqrtf((distanceVector.x * distanceVector.x) + (distanceVector.y * distanceVector.y));

	return distanceVector / magnitude;
}

float RayLine::getDistance() const 
{
	const sf::Vector2f distanceVector = t_points[1] - t_points[0];
	return sqrtf((distanceVector.x * distanceVector.x) + (distanceVector.y * distanceVector.y));
}

void RayLine::setColor(sf::Color t_color)
{
	m_line.clear();
	m_line.append({ t_points[0], t_color });
	m_line.append({ t_points[1], t_color });
}

void RayLine::setStartPoint(sf::Vector2f t_position)
{
	t_points[0] = t_position;
	sf::Color rayColor = m_line[0].color;

	m_line.clear();
	m_line.append({ t_points[0], rayColor });
	m_line.append({ t_points[1], rayColor });
}

void RayLine::setEndPoint(sf::Vector2f t_position)
{
	t_points[1] = t_position;
	sf::Color rayColor = m_line[0].color;

	m_line.clear();
	m_line.append({ t_points[0], rayColor });
	m_line.append({ t_points[1], rayColor });
}
