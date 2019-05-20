#pragma once

#include <SFML/Graphics.hpp>

class FontDependant
{
public:
	FontDependant()
		: m_font(nullptr)
	{}



protected:
	const sf::Font* m_font;
};

