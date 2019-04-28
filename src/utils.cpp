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
