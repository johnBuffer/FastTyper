#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <transition.hpp>
#include "rounded_rectangle.hpp"
#include "utils.hpp"
#include "theme.hpp"

#include <iostream>

class PopUpLabel : public sf::Drawable
{
public:
	PopUpLabel(const std::string& name, float x, float y, const sf::Font& font, bool dual = false)
		: m_name(name)
		, m_x(x)
		, m_start_y(y)
		, m_y(y)
		, m_font(font)
		, m_char_size(12)
		, m_radius(2.0f)
		, m_retract(false)
		, m_dual(dual)
	{
		m_width = getTextWidth(m_font, m_char_size, m_name) + 2.0f * m_radius;

		const float fact = dual ? 2.0f : 1.0f;
		m_height = 2.0f*(m_radius + fact * font.getLineSpacing(m_char_size));
		m_y = y - m_height * 0.5f;
		m_y.setSpeed(12.0f);

		m_clock.restart();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::Text text = textBuilder(m_font, m_char_size, Theme<>::LetterSkipped, m_name);

		RoundedRectangle back(m_width, m_height, m_radius, m_x, m_y);
		back.setFillColor(sf::Color(200, 200, 200));
		
		text.setPosition(m_x + m_radius, m_y + m_radius);

		target.draw(back);
		target.draw(text);
	}

	bool isDual() const
	{
		return m_dual;
	}

	void update()
	{
		const float delay(1.5f);
		if (m_clock.getElapsedTime().asSeconds() >= delay && !m_retract)
		{
			m_y = m_start_y;
			m_retract = true;
		}
	}

	bool done() const
	{
		return m_retract && (m_y == m_start_y);
	}

private:
	const std::string m_name;

	float m_width;
	float m_height;
	bool  m_dual;

	const float m_x;
	const float m_start_y;
	trn::Transition<float> m_y;

	const sf::Font& m_font;
	const uint32_t  m_char_size;
	const float     m_radius;

	sf::Clock m_clock;
	bool m_retract;
};

