#pragma once

#include <SFML/Graphics.hpp>

class FontDependant
{
public:
	FontDependant()
		: m_font(nullptr)
		, m_char_size(0)
	{}

	FontDependant(const sf::Font& font, uint32_t char_size = 0)
		: m_font(&font)
		, m_char_size(char_size)
	{}

	virtual void setFont(const sf::Font& font, uint32_t char_size)
	{
		m_font = &font;
		m_char_size = char_size;
	}

protected:
	const sf::Font* m_font;
	uint32_t m_char_size;

	const sf::Font& getFont() const
	{
		return *m_font;
	}
};

