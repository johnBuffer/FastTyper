#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Word
{
public:
	Word(const std::string& str);

	bool addChar(char c);
	bool isValid(char c) const;
	void setInvalid(bool b);

	void update(float dt);

	void render(sf::RenderTarget& target, sf::Text& text) const;

private:
	const std::string m_string;
	uint32_t m_current_char;
	bool m_valid;

};
