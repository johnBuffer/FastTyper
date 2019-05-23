#pragma once

#include <SFML/Graphics.hpp>

class FontDependant
{
public:
	FontDependant()
		: m_font(nullptr)
	{}

	virtual void setFont(const sf::Font& font)
	{
		m_font = &font;
	}

protected:
	const sf::Font* m_font;

	const sf::Font& getFont() const
	{
		return *m_font;
	}
};

