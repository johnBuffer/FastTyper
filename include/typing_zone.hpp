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
		, m_input_zone(m_width, m_height, x, y)
	{}

	void init(uint32_t character_size, const sf::Text& text)
	{
		m_input_zone.init(character_size, text);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_input_zone);
	}

	InputZone& getInput()
	{
		return m_input_zone;
	}

	const std::string& getTyped() const
	{
		return m_input_zone.getTyped();
	}

private:
	const float m_side_width = 50.0f;

	float m_width;
	float m_height;
	float m_x;
	float m_y;

	InputZone m_input_zone;
};

