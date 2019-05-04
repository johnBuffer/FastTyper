#pragma once

#include "gauge.hpp"
#include "challenge_status.hpp"

class StatsDisplayer : public sf::Drawable, public Rectangle
{
public:
	StatsDisplayer() 
		: Rectangle()
		, m_wpm()
		, m_acc()
		, m_tle()
		, m_ene()
	{}

	StatsDisplayer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_wpm()
		, m_acc()
		, m_tle()
		, m_ene()
	{
		const float gauge_width(600.0f);
		const float center_offset((width - gauge_width)*0.5f);
		const float margin(16.0f);
		const float gauge_height(24.0f);
		const float full_height(margin + gauge_height);
		const float gauge_x(center_offset);

		m_wpm.setSize(gauge_width, gauge_height);
		m_wpm.setPosition(gauge_x, y);
		m_wpm.setBackColor(Theme<>::Color2);
		m_wpm.setFillColor(Theme<>::Color1);

		m_acc.setSize(gauge_width, gauge_height);
		m_acc.setPosition(gauge_x, y + full_height);
		m_acc.setBackColor(Theme<>::Color2);
		m_acc.setFillColor(Theme<>::LetterCorrected);
		m_acc.setDecimals(0);

		m_tle.setSize(gauge_width, gauge_height);
		m_tle.setPosition(gauge_x, y + 2.0f*full_height);
		m_tle.setBackColor(Theme<>::Color2);
		m_tle.setFillColor(Theme<>::LetterWrong);

		m_ene.setSize(gauge_width, gauge_height);
		m_ene.setPosition(gauge_x, y + 3.0f*full_height);
		m_ene.setBackColor(Theme<>::Color2);
		m_ene.setFillColor(Theme<>::LetterOk);
		m_ene.setDecimals(0);
	}

	void setFont(const sf::Font& font)
	{
		m_wpm.setFont(font);
		m_acc.setFont(font);
		m_tle.setFont(font);
		m_ene.setFont(font);

		m_wpm.setLabel("WPM");
		m_acc.setLabel("Accuracy");
		m_tle.setLabel("Time Since Last Error");
		m_ene.setLabel("Perfect Streak");
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_wpm);
		target.draw(m_acc);
		target.draw(m_tle);
		target.draw(m_ene);
	}

	void drawBloom(sf::RenderTarget& target) const
	{
		m_wpm.drawBloom(target);
		m_acc.drawBloom(target);
		m_tle.drawBloom(target);
		m_ene.drawBloom(target);
	}

	void update(const ChallengeStatus& status)
	{
		m_wpm.setValue(status.getWPM());
		m_acc.setValue(100.0f * status.getAccuracy());
		m_tle.setValue(status.last_error.getElapsedTime().asMilliseconds() * 0.001f);
		m_ene.setValue(status.entry_no_error);
	}

private:
	Gauge m_wpm;
	Gauge m_acc;
	Gauge m_tle;
	Gauge m_ene;

	sf::Text m_wpm_label;
};
