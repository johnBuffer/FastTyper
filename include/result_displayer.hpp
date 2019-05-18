#pragma once

#include <SFML/Graphics.hpp>
#include "transition.hpp"
#include "utils.hpp"
#include "labeled_value.hpp"

class ResultDisplayer : public sf::Drawable
{
public:
	ResultDisplayer(float x, float y)
		: m_x(x)
		, m_y(y)
	{

	}

	void setFont(const sf::Font& font)
	{
		m_font = &font;
	}

	void setY(float y)
	{
		m_y = y;
	}

	void setValue(uint32_t correct, uint32_t perfect)
	{
		m_correct_words = correct;
		m_perfect_words = perfect;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const uint32_t char_size(64);

		const std::string correct_str("Correct words");
		const std::string correct_val(toString(m_correct_words, 0));

		const std::string perfect_str("Perfect words");
		const std::string perfect_val(toString(m_perfect_words, 0));

		LabeledValue correct_words(correct_str, correct_val, char_size);
		correct_words.setFont(*m_font);
		correct_words.setCenter(true);
		LabeledValue perfect_words(perfect_str, perfect_val, char_size);
		perfect_words.setFont(*m_font);
		perfect_words.setCenter(true);

		correct_words.setPosition(m_x, m_y);

		target.draw(correct_words);
	}

private:
	trn::Transition<float> m_x;
	trn::Transition<float> m_y;

	const sf::Font* m_font;

	uint32_t m_correct_words;
	uint32_t m_perfect_words;
};
