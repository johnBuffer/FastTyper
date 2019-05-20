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
		, m_perfect_words(0)
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
		const float margin(20.0f);
		const uint32_t char_size(50);

		sf::Text result_label = textBuilder(*m_font, char_size*0.75, sf::Color::White, "Results");
		result_label.setPosition(0.0f, m_y);
		setRearX(result_label, m_x - 2.0f * margin);

		const std::string correct_str("Correct words");
		const std::string correct_val(toString(m_correct_words, 0));

		const std::string perfect_str("Perfect words");
		const std::string perfect_val(toString(m_perfect_words, 0));

		LabeledValue correct_words(correct_str, correct_val, char_size);
		correct_words.setFont(*m_font);
		LabeledValue perfect_words(perfect_str, perfect_val, char_size);
		perfect_words.setFont(*m_font);

		const float labeled_value_height(correct_words.getHeight());
		const float y_delta(labeled_value_height + margin);
		sf::RectangleShape separator(sf::Vector2f(1.0f, 2.0f * labeled_value_height + margin));
		separator.setFillColor(Theme<>::LetterSkipped);
		separator.setPosition(m_x - margin, m_y);

		correct_words.setPosition(m_x, m_y);
		perfect_words.setPosition(m_x, m_y + y_delta);

		target.draw(result_label);
		target.draw(correct_words);
		target.draw(perfect_words);
		target.draw(separator);
	}

private:
	trn::Transition<float> m_x;
	trn::Transition<float> m_y;

	const sf::Font* m_font;

	uint32_t m_correct_words;
	uint32_t m_perfect_words;
};
