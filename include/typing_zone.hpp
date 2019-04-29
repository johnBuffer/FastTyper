#pragma once

#include "input_zone.hpp"
#include "chart.hpp"
#include "utils.hpp"

class TypingZone : public sf::Drawable
{
public:
	TypingZone(float width, float height, float x, float y)
		: m_width(width)
		, m_height(height)
		, m_x(x)
		, m_y(y)
		, m_histo_wpm(width - 2.0f * m_side_width, height * 0.5f, m_side_width, y)
		, m_histo_acc(width - 2.0f * m_side_width, height * 0.5f, m_side_width, y)
		, m_input_zone(m_width, m_height, x, y)
	{}

	void init(uint32_t character_size, const sf::Text& text)
	{
		m_input_zone.init(character_size, text);
	}

	void setFont(const sf::Font& font)
	{
		const float wpm_x(0.0f);
		const float wpm_y(0.0f);

		m_wpm_label = textBuilder(font, 24, Theme<>::Color1, "WPM");
		m_wpm_label.setPosition(wpm_x, wpm_y);

		m_acc_label = textBuilder(font, 24, Theme<>::Color1, "Accuracy");
		m_acc_label.setPosition(wpm_x, wpm_y);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{	
		target.draw(m_wpm_label);
		target.draw(m_acc_label);

		/*const float acc_x1(m_width - (m_width - 800.0f)*0.5f - text.getGlobalBounds().width);
		const float acc_y1(wpm_y);
		text.setPosition(acc_x1, acc_y1);
		target.draw(text);*/

		/*text.setCharacterSize(48);
		text.setString(toString(100.0f * getAccuracy(), 0) + '%');
		const float acc_x2(m_width - (m_width - 800.0f)*0.5f - text.getGlobalBounds().width);
		const float acc_y2(wpm_y + 30.0f);
		text.setPosition(acc_x2, acc_y2);
		target.draw(text);*/

		target.draw(m_input_zone);
	}

	InputZone& getInput()
	{
		return m_input_zone;
	}

private:
	const float m_side_width = 50.0f;

	float m_width;
	float m_height;
	float m_x;
	float m_y;

	mutable Chart m_histo_wpm;
	mutable Chart m_histo_acc;

	sf::Text m_wpm_label;
	sf::Text m_acc_label;

	sf::Text m_text_values;
	InputZone m_input_zone;
};

