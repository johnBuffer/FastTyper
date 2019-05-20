#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "utils.hpp"

class LabeledValue : public sf::Drawable
{
public:
	LabeledValue(const std::string& label, const std::string& value, uint32_t char_size)
		: m_label(label)
		, m_value(value)
		, m_x(0.0f)
		, m_y(0.0f)
		, m_char_size(char_size)
		, m_center(false)
		, m_font(nullptr)
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
		m_font = &font;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const sf::Font& font(*m_font);
		const float label_char_size(m_char_size * 0.25f);

		sf::Text label_text(textBuilder(font, label_char_size, sf::Color::White, m_label));
		sf::Text value_text(textBuilder(font, m_char_size, sf::Color::White, m_value));
	
		float x(m_x);
		float y_value(m_y + font.getLineSpacing(label_char_size));

		if (m_center)
		{
			const float width(std::max(value_text.getGlobalBounds().width, label_text.getGlobalBounds().width));
			x -= width * 0.5f;
		}

		label_text.setPosition(x, m_y);
		value_text.setPosition(x, y_value);

		target.draw(label_text);
		target.draw(value_text);
	}

	float getHeight() const
	{
		const sf::Font& font(*m_font);
		const float label_char_size(m_char_size * 0.25f);

		return font.getLineSpacing(label_char_size) + font.getLineSpacing(m_char_size);
	}

private:
	std::string m_label;
	std::string m_value;

	float m_x;
	float m_y;

	bool m_center;

	const sf::Font* m_font;
	uint32_t m_char_size;
};
