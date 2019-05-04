#include "utils.hpp"

sf::Text textBuilder(const sf::Font& font, uint32_t char_size, const sf::Color& color, const std::string& str)
{
	sf::Text text;

	text.setFont(font);
	text.setCharacterSize(char_size);
	text.setFillColor(color);
	text.setString(str);

	return text;
}

void setRearX(sf::Text& text, float x)
{
	const float width(text.getGlobalBounds().width);
	text.setPosition(x - width, text.getPosition().y);
}
