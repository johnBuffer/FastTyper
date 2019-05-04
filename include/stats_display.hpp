#pragma once

#include "gauge.hpp"

class StatsDisplayer : public sf::Drawable, public Rectangle
{
public:
	StatsDisplayer() 
		: Rectangle()
		, m_label_width(50.0f)
		, m_wpm()
		, m_acc()
		, m_tle()
	{}

	StatsDisplayer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_label_width(75.0f)
		, m_wpm()
		, m_acc()
		, m_tle()
	{
		const float margin(16.0f);
		const float gauge_height(16.0f);
		const float full_height(margin + gauge_height);

		m_wpm.setSize(400.0f, gauge_height);
		m_wpm.setPosition(m_x + m_label_width, y);
		m_wpm.setBackColor(Theme<>::Color2);

		m_acc.setSize(400.0f, gauge_height);
		m_acc.setPosition(m_x + m_label_width, y + full_height);
		m_acc.setBackColor(Theme<>::Color2);

		m_tle.setSize(400.0f, gauge_height);
		m_tle.setPosition(m_x + m_label_width, y + 2.0f*full_height);
		m_tle.setBackColor(Theme<>::Color2);
	}

	void setFont(const sf::Font& font)
	{
		m_wpm_label = textBuilder(font, 20, Theme<>::Color2, "WPM");
		m_wpm_label.setPosition(m_x, m_y);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_wpm_label);

		target.draw(m_wpm);
		target.draw(m_acc);
		target.draw(m_tle);
	}

	void setWpmValue(float value)
	{
		m_wpm.setValue(value);
	}

private:
	const float m_label_width;
	Gauge m_wpm;
	Gauge m_acc;
	Gauge m_tle;

	sf::Text m_wpm_label;
};
