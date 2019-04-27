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
	{
		m_x.setSpeed(8.0f);
		m_y.setSpeed(8.0f);

		std::cout << m_x << " " << m_y << std::endl;
	}

	void init(uint32_t character_size, const sf::Text& text)
	{
		m_text = text;
		m_text.setCharacterSize(character_size);
	}

	void add(char c, float offset_y)
	{
		sf::Vector2f pos(m_width, offset_y);
		m_letters.emplace_back(c, pos, 0U, m_text);
		m_letters.back().setY(0.0f);
		m_width += m_letters.back().getAdvance();
	}

	void pop()
	{
		m_width -= m_letters.back().getAdvance();
		m_letters.pop_back();
	}

	void clear()
	{
		m_width = 0.0f;
		m_letters.clear();
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

		//std::cout << m_x << " " << m_y << std::endl;
	}

private:
	sf::Text m_text;
	trn::Transition<float> m_x, m_y;
	std::vector<Letter> m_letters;

	float m_width;
};