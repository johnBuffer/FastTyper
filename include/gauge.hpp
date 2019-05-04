#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "rounded_rectangle.hpp"

class Gauge : public Rectangle, public sf::Drawable
{
public:
	Gauge()
		: Rectangle()
		, m_margin(10.0f)
		, m_max_value(0.0f)
		, m_min_value(0.0f)
		, m_current_value(0.0f)
		, m_decimals(1)
		, m_fill_color(sf::Color::White)
		, m_back_color(sf::Color::White)
	{}

	Gauge(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_margin(50.0f)
		, m_max_value(0.0f)
		, m_min_value(0.0f)
		, m_current_value(0.0f)
		, m_decimals(1)
		, m_fill_color(sf::Color::White)
		, m_back_color(sf::Color::White)
	{}

	void setFillColor(const sf::Color& color)
	{
		m_fill_color = color;
	}

	void setBackColor(const sf::Color& color)
	{
		m_back_color = color;
	}

	void setFont(const sf::Font& font)
	{
		m_label = textBuilder(font, 16, sf::Color::White, "Label");
		m_label.setPosition(m_x + m_margin, m_y);

		m_max_text = textBuilder(font, 16, sf::Color::White, "");
		m_max_text.setPosition(m_x + m_margin + m_width - 75.0f, m_y);

		m_value_text = textBuilder(font, 32, sf::Color::White, "");
		m_value_text.setPosition(m_x, m_y);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_label);
		target.draw(m_value_text);
		target.draw(m_max_text);

		const float margin(4.0f);
		const float height(14.0f);
		const float x(m_x + m_margin);
		const float y(m_y + 20.0f);

		RoundedRectangle back(m_width, height, 4.0f, x, y);
		back.setFillColor(m_back_color);
		target.draw(back);

		const float radius(4.0f);
		const float current_width((m_width - 2.0f * margin) * m_current_value / (m_max_value-m_min_value));
		if (current_width >= radius)
		{
			RoundedRectangle in(current_width, height - 2.0f*margin, 2.0f, x + margin, y + margin);
			in.setFillColor(m_fill_color);
			target.draw(in);
		}
	}

	void drawBloom(sf::RenderTarget& target) const
	{
		const float margin(4.0f);
		const float height(14.0f);
		const float x(m_x + m_margin);
		const float y(m_y + 20.0f);

		const float radius(4.0f);
		const float current_width((m_width - 2.0f * margin) * m_current_value / (m_max_value - m_min_value));
		if (current_width >= radius)
		{
			RoundedRectangle in(current_width, height - 2.0f*margin, 2.0f, x + margin, y + margin);
			in.setFillColor(m_fill_color);
			target.draw(in);
		}
	}

	void setValue(float value)
	{
		m_current_value = value;
		m_max_value = value > m_max_value ? value : m_max_value;
		m_min_value = value < m_min_value ? value : m_min_value;

		m_max_text.setString("MAX  " + toString(m_max_value, m_decimals));
		setRearX(m_max_text, m_x + m_margin + m_width - 12.0f);

		m_value_text.setString(toString(m_current_value, m_decimals));
		setRearX(m_value_text, m_x + m_margin - 12.0f);
	}

	void setLabel(const std::string& label)
	{
		m_label.setString(label);
	}

	void setDecimals(uint32_t count)
	{
		m_decimals = count;
	}

private:
	const float m_margin;
	float m_max_value;
	float m_min_value;
	float m_current_value;

	uint32_t m_decimals;

	sf::Color m_fill_color;
	sf::Color m_back_color;

	sf::Text m_label;
	sf::Text m_value_text;
	sf::Text m_max_text;
};


