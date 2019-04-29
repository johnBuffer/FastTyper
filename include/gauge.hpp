#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"

class Gauge : public Rectangle, public sf::Drawable
{
public:
	Gauge()
		: Rectangle()
	{}

	Gauge(float width, float height)
	{

	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
	}

private:
	float m_max_value;
	float m_min_value;
	float m_current_value;
};


