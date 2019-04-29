#pragma once

#include "rectangle.hpp"
#include <SFML/Graphics.hpp>
#include "chart.hpp"

class MetricVisualizer : public Rectangle,  public sf::Drawable
{
public:
	MetricVisualizer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_wpm(0.0f, 0.0f, x, y)
		, m_acc(0.0f, 0.0f, 0.0f, 0.0f)
	{
		m_wpm.setSize(width, )
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_wpm);
		target.draw(m_acc);
	}

	void addValues(float wpm, float acc)
	{
		m_wpm.addValue(wpm, 1.0f);
		m_acc.addValue(acc, 1.0f);
	}

private:
	const float m_margin = 8.0f;

	Chart m_wpm;
	Chart m_acc;

};

