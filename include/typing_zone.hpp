#pragma once

#include "input_zone.hpp"
#include "chart.hpp"

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

	void setFont(const sf::Font& font)
	{
		const float wpm_x(0.0f);
		const float wpm_y(0.0f);

		m_wpm_label.setFont(font);
		m_wpm_label.setFillColor(Theme<>::Color1);
		m_wpm_label.setCharacterSize(24);
		m_wpm_label.setPosition(wpm_x, wpm_y);
		m_wpm_label.setString("WPM");

		m_acc_label.setFont(font);
		m_acc_label.setFillColor(Theme<>::LetterWrong);
		m_acc_label.setCharacterSize(24);
		m_acc_label.setPosition(wpm_x, wpm_y);
		m_acc_label.setString("WPM");
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{	
		target.draw(m_wpm_label);

		text.setCharacterSize(48);
		text.setPosition(wpm_x, wpm_y + 30.0f);
		text.setString(toString(getWPM(), 0));
		target.draw(text);

		text.setFillColor(Theme<>::LetterWrong);
		text.setCharacterSize(24);
		text.setString("Accuracy");
		const float acc_x1(m_width - (m_width - 800.0f)*0.5f - text.getGlobalBounds().width);
		const float acc_y1(wpm_y);
		text.setPosition(acc_x1, acc_y1);
		target.draw(text);

		text.setCharacterSize(48);
		text.setString(toString(100.0f * getAccuracy(), 0) + '%');
		const float acc_x2(m_width - (m_width - 800.0f)*0.5f - text.getGlobalBounds().width);
		const float acc_y2(wpm_y + 30.0f);
		text.setPosition(acc_x2, acc_y2);
		target.draw(text);
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

