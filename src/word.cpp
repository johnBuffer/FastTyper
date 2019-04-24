#include "word.hpp"

Word::Word(const std::string& str) :
	m_string(str),
	m_current_char(0),
	m_valid(true)
{
}

bool Word::addChar(char c)
{
	if (c == m_string[m_current_char])
	{
		++m_current_char;
		return true;
	}

	return false;
}

bool Word::isValid(char c) const
{
	return m_string[m_current_char] == c;
}

void Word::setInvalid(bool b)
{
	m_valid = b;
}

void Word::render(sf::RenderTarget& target, sf::Text& text) const
{
}
