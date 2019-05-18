#include "utils.hpp"
#include <fstream>

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

void showHelp(float x, float y, const sf::Font& font, sf::RenderTarget& target)
{
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(20);

	const float text_y(y);
	text.setPosition(x, text_y);
	text.setString("TAB - Export replay");

	target.draw(text);
}

bool exists(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

float getTextWidth(const sf::Font& font, uint32_t char_size, const std::string & str)
{
	float width(0.0f);

	for (const char c : str)
	{
		width += font.getGlyph(c, char_size, false).advance;
	}

	return width;
}
