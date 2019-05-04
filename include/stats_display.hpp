#pragma once

#include "gauge.hpp"

class StatsDisplayer : public sf::Drawable, public Rectangle
{
public:
	StatsDisplayer() 
		: Rectangle()
		, m_wpm()
		, m_acc()
		, m_tle()
	{}

	StatsDisplayer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_wpm()
		, m_acc()
		, m_tle()
	{
		const float gauge_width(400.0f);
		const float center_offset((width - gauge_width)*0.5f);
		const float margin(16.0f);
		const float gauge_height(24.0f);
		const float full_height(margin + gauge_height);
		const float gauge_x(center_offset);

		m_wpm.setSize(gauge_width, gauge_height);
		m_wpm.setPosition(gauge_x, y);
		m_wpm.setBackColor(Theme<>::Color2);

		m_acc.setSize(gauge_width, gauge_height);
		m_acc.setPosition(gauge_x, y + full_height);
		m_acc.setBackColor(Theme<>::Color2);
		m_acc.setDecimals(0);

		m_tle.setSize(gauge_width, gauge_height);
		m_tle.setPosition(gauge_x, y + 2.0f*full_height);
		m_tle.setBackColor(Theme<>::Color2);
	}

	void setFont(const sf::Font& font)
	{
		m_wpm.setFont(font);
		m_acc.setFont(font);
		m_tle.setFont(font);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_wpm);
		target.draw(m_acc);
		target.draw(m_tle);
	}

	void setWpmValue(float value)
	{
		m_wpm.setValue(value);
	}

	void setAccValue(float value)
	{
		m_acc.setValue(value);
	}

private:
	Gauge m_wpm;
	Gauge m_acc;
	Gauge m_tle;

	sf::Text m_wpm_label;
};
