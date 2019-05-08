#pragma once

#include <SFML/Graphics.hpp>
#include "circle_ratio.hpp"

class CircleClock : public sf::Drawable
{
public:
	CircleClock(float radius, float x, float y, float max_time)
		: m_x(x)
		, m_y(y)
		, m_circle_ratio(radius, x, y, 1.0f)
		, m_circle_back(radius, x, y, 1.0f)
		, m_max_time(max_time)
		, m_started(false)
		, m_start_time(0)
	{
		m_circle_back.setFillColor(Theme<>::LetterSkipped);
	}

	void setFont(const sf::Font& font)
	{
		m_font = font;
	}

	void start()
	{
		m_clock.restart();
		m_started = true;
	}

	void reset()
	{
		m_started = false;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		m_circle_ratio.setRatio(getRatio());
		target.draw(m_circle_back, states);
		target.draw(m_circle_ratio, states);

		sf::Text text;
		text.setFont(m_font);
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(70);
		text.setString(toString(getRemainingTime(), 0));
		const float text_x(m_x - text.getGlobalBounds().width * 0.5f);
		text.setPosition(text_x, m_y - 50.0f);
		target.draw(text);
	}

	void setFillColor(const sf::Color& color)
	{
		m_circle_ratio.setFillColor(color);
	}

private:
	float m_x;
	float m_y;
	float m_max_time;
	float m_start_time;

	mutable CircleRatio m_circle_ratio;
	CircleRatio m_circle_back;

	bool m_started;
	sf::Clock m_clock;
	
	sf::Font m_font;

	float getRemainingTime() const
	{
		if (m_started)
		{
			return m_max_time - 0.001f * m_clock.getElapsedTime().asMilliseconds();
		}
		
		return m_max_time;
	}
	
	float getRatio() const
	{
		const float ratio(getRemainingTime() / m_max_time);
		return ratio;
	}
};


