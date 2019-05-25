#pragma once
#include <vector>
#include <numeric>
#include <sstream>
#include <SFML/Graphics.hpp>


template<typename T>
std::string numberToString(const T& n)
{
	std::stringstream sx;
	sx << n;
	return sx.str();
}

template<typename T>
std::string toString(const T& number, uint32_t decimals)
{
	std::string result;
	std::stringstream sx;
	sx << std::fixed << number;
	result = sx.str();

	size_t pos = result.find('.');
	if (pos == std::string::npos)
		return result;
	else if (!decimals)
	{
		return result.substr(0, pos);
	}
	else
	{
		if (pos + decimals + 1 < result.size())
		{
			return result.substr(0, pos + decimals + 1);
		}
	}

	return result;
}

sf::Text textBuilder(const sf::Font& font, uint32_t char_size, const sf::Color& color, const std::string& str);

void setRearX(sf::Text& text, float x);

void showHelp(float x, float y, const sf::Font& font, sf::RenderTarget& target, bool done);

bool exists(const std::string& name);

float getTextWidth(const sf::Font& font, uint32_t char_size, const std::string& str);

template<typename T>
const T& getRandomElemFromVector(const std::vector<T>& vector)
{
	const uint32_t vector_size(vector.size());
	const uint32_t rand_index(rand() % vector_size);

	return vector[rand_index];
}

const char getLastCharOfString(const std::string& str);

const bool isValidChar(char c);

const char getCharFromUnicode(uint32_t unicode);

