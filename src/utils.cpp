#include "utils.hpp"
#include <fstream>
#include "rounded_rectangle.hpp"
#include "theme.hpp"

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

void showHelp(float x, float y, const sf::Font& font, sf::RenderTarget& target, bool done)
{
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(20);

	const float radius(4.0f);
	const float text_y(y);
	text.setString("Tab");
	text.setPosition(x, text_y);
	text.setFillColor(sf::Color::Black);

	const float width(text.getLocalBounds().width);
	const float height(font.getLineSpacing(20));

	RoundedRectangle back(width + 3.0f * radius, height + 2.0f * radius, radius, x - radius, y - radius);
	back.setFillColor(sf::Color::White);

	if (done)
	{
		RoundedRectangle back_done(204.0f, 40.0f, 8.0f, x - 8.0f, y - 8.0f);
		back_done.setFillColor(Theme<>::LetterOk);
		target.draw(back_done);
	}

	target.draw(back);
	target.draw(text);
	
	text.setString("Export replay");
	text.setPosition(x + width + 4.0f*radius, text_y);
	text.setFillColor(sf::Color::White);

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
	float max_width(0.0f);

	for (const char c : str)
	{
		if (c == '\n') {
			max_width = std::max(max_width, width);
			width = 0.0f;
		}
		else {
			width += font.getGlyph(c, char_size, false).advance;
		}
	}

	return std::max(max_width, width);
}

const char getLastCharOfString(const std::string& str)
{
	const std::size_t str_size(str.size());
	if (str_size) {
		return str[str_size - 1];
	}

	return '\0';
}

const bool isValidChar(char c)
{
	return (c < 'z' && c > '\'');
}
