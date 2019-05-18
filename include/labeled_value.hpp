#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "utils.hpp"

class LabeledValue : public sf::Drawable
{
public:
	LabeledValue(const std::string& label, const std::string& value)
		: m_label(label)
		, m_value(value)
		, m_x(0.0f)
		, m_y(0.0f)
	{}

	void setPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	void setCenter(bool b)
	{
		m_center = b;
	}

	void setFont(const sf::Font& font)
	{
		m_font = font;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const float label_char_size(m_char_size * 0.5f);
		sf::Text label_text(textBuilder(m_font, label_char_size, sf::Color::White, m_label));
		sf::Text value_text(textBuilder(m_font, m_char_size, sf::Color::White, m_value));
	
		float x(m_x);
		float y_value(m_y + m_font.getLineSpacing(label_char_size));

		if (m_center)
		{
			const float width(value_text.getGlobalBounds().width);
			x -= width * 0.5f;
		}

		label_text.setPosition(x, m_y);
		value_text.setPosition(x, y_value);

		target.draw(label_text);
		target.draw(value_text);
	}

private:
	std::string m_label;
	std::string m_value;

	float m_x;
	float m_y;

	bool m_center;

	sf::Font m_font;
	uint32_t m_char_size;
};
