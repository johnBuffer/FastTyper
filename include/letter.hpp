#pragma once

#include <SFML/Graphics.hpp>

class Letter
{
public:
	enum LetterState
	{
		Ok,
		Wrong,
		Unknown,
	};

	Letter(char c, const sf::Vector2f& position, uint32_t line, const sf::Text& text) 
		: m_char(c)
		, m_text(text)
		, m_line(line)
		, m_state(Letter::Unknown)
	{
		m_text.setPosition(position);
		m_text.setString(c);
		m_bounds = m_text.getGlobalBounds();
	}

	const sf::FloatRect& getBounds() const
	{
		return m_bounds;
	}

	uint32_t getLine() const
	{
		return m_line;
	}

	void setPosition(const sf::Vector2f& position)
	{
		m_text.setPosition(position);
	}

	void addLine()
	{
		++m_line;
	}

	void draw(sf::RenderTarget& target) const
	{
		target.draw(m_text);
	}

private:
	const char    m_char;
	sf::Text      m_text;
	uint32_t      m_line;
	LetterState   m_state;
	sf::FloatRect m_bounds;
};
