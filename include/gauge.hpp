#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "rounded_rectangle.hpp"

class Gauge : public Rectangle, public sf::Drawable
{
public:
	Gauge()
		: Rectangle()
		, m_fill_color(sf::Color::White)
		, m_back_color(sf::Color::White)
	{}

	Gauge(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_fill_color(sf::Color::White)
		, m_back_color(sf::Color::White)
	{

	}

	void setFillColor(const sf::Color& color)
	{
		m_fill_color = color;
	}

	void setBackColor(const sf::Color& color)
	{
		m_back_color = color;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const float margin(4.0f);
		RoundedRectangle back(m_width, m_height, 4.0f, m_x, m_y);
		back.setFillColor(m_back_color);
		target.draw(back);

		const float radius(4.0f);
		const float current_width((m_width - 2.0f * margin) * m_current_value / (m_max_value-m_min_value));
		if (current_width >= radius)
		{
			RoundedRectangle in(current_width, m_height - 2.0f*margin, 4.0f, m_x + margin, m_y + margin);
			in.setFillColor(m_fill_color);
			target.draw(in);
		}
	}

	void setValue(float value)
	{
		m_current_value = value;
		m_max_value = value > m_max_value ? value : m_max_value;
		m_min_value = value < m_min_value ? value : m_min_value;
	}

private:
	float m_max_value;
	float m_min_value;
	float m_current_value;

	sf::Color m_fill_color;
	sf::Color m_back_color;
};


