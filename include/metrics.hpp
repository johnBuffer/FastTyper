#pragma once

#include "rectangle.hpp"
#include <SFML/Graphics.hpp>
#include "chart.hpp"

class MetricVisualizer : public Rectangle,  public sf::Drawable
{
public:
	MetricVisualizer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_margin_x(250.0f)
		, m_margin_y(8.0f)
		, m_wpm()
		, m_acc()
		, m_threshold(48)
		, m_last_added(0)
	{
		const float chart_width((m_width - 2.0f*m_margin_x));
		const float chart_height((m_height - m_margin_y) * 0.5f);
		m_wpm.setSize(chart_width, chart_height);
		m_acc.setSize(chart_width, chart_height);

		m_wpm.setPosition(x + m_margin_x, y);
		m_acc.setPosition(x + m_margin_x, y + chart_height + m_margin_y);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_wpm);
		target.draw(m_acc);
	}

	void addValues(float wpm, float acc, uint32_t ms)
	{
		if (ms - m_last_added >= m_threshold)
		{
			m_wpm.addValue(wpm, 1.0f);
			m_acc.addValue(acc, 1.0f);

			m_last_added = ms;
		}
	}

private:
	const float m_margin_x;
	const float m_margin_y;

	Chart m_wpm;
	Chart m_acc;

	uint32_t m_threshold;
	uint32_t m_last_added;
};

