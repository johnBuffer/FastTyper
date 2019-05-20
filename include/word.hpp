#pragma once

#include "letter.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Word : public sf::Drawable
{
public:
	Word(float x, float y)
		: m_x(x)
		, m_y(y)
		, m_width(0.0f)
		, m_str("")
	{
		m_x.setSpeed(8.0f);
		m_y.setSpeed(8.0f);
	}

	void init(uint32_t character_size, const sf::Font& font)
	{
		m_font = &font;
	}

	void add(char c, float offset_y)
	{
		m_str += c;
		sf::Vector2f pos(m_width, offset_y);
		m_letters.emplace_back(c, pos, 0U, m_text);
		m_letters.back().setY(0.0f);
		m_width += m_letters.back().getAdvance();
	}

	void pop()
	{
		m_width -= m_letters.back().getAdvance();
		m_letters.pop_back();
		m_str = m_str.substr(0, m_str.size() - 1);
	}

	void clear()
	{
		m_width = 0.0f;
		m_letters.clear();
		m_str = std::string();
	}

	const std::string& getString() const
	{
		return m_str;
	}

	void center(uint32_t width)
	{
		m_x = (width - m_width)*0.5f;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::RenderStates rs;
		rs.transform.combine(states.transform);
		rs.transform.translate(m_x, m_y);
		for (const Letter& letter : m_letters)
		{
			target.draw(letter, rs);
		}
	}

private:
	const sf::Font*        m_font;
	trn::Transition<float> m_x, m_y;
	std::vector<Letter>    m_letters;
	std::string            m_str;
	float                  m_width;
};


