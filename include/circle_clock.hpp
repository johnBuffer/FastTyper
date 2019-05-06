#pragma once

#include <SFML/Graphics.hpp>
#include "circle_ratio.hpp"

class CircleClock : public sf::Drawable
{
public:
	CircleClock(float radius, float x, float y, float ratio)
		: m_circle_ratio(radius, x, y, ratio)
		, m_circle_back(radius, x, y, 1.0f)
	{
		m_circle_back.setFillColor(Theme<>::LetterSkipped);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_circle_back, states);
		target.draw(m_circle_ratio, states);
	}

	void setFillColor(const sf::Color& color)
	{
		m_circle_ratio.setFillColor(color);
	}

private:
	float m_radius;
	float m_x;
	float m_y;

	CircleRatio m_circle_ratio;
	CircleRatio m_circle_back;
};
