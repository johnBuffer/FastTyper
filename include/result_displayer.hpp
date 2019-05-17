#pragma once

#include <SFML/Graphics.hpp>
#include "transition.hpp"
#include "utils.hpp"

class ResultDisplayer : public sf::Drawable
{
public:
	ResultDisplayer()
	{

	}

	void setFont(const sf::Font& font)
	{
		m_font = font;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const uint32_t char_size(32);

		const std::string correct_str("Correct words: " + toString(m_correct_words, 0));
		const std::string perfect_str("Perfect words: " + toString(m_perfect_words, 0));

		sf::Text text = textBuilder(m_font, char_size, sf::Color::White, correct_str);
		sf::Text text = textBuilder(m_font, char_size, sf::Color::White, perfect_str);


	}

private:
	trn::Transition<float> m_x;
	trn::Transition<float> m_y;

	sf::Font m_font;

	uint32_t m_correct_words;
	uint32_t m_perfect_words;
};
