#pragma once

#include "input_zone.hpp"
#include "chart.hpp"
#include "utils.hpp"
#include "rectangle.hpp"

class TypingZone : public sf::Drawable, public Rectangle, public FontDependant
{
public:
	TypingZone(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, m_input_zone(m_width, m_height, x, y)
	{}

	void setFont(const sf::Font& font, uint32_t character_size) override
	{
		FontDependant::setFont(font, character_size);
		m_input_zone.setFont(font, character_size);
	}

	void showInstruction(sf::RenderTarget& target, const sf::Color& color) const
	{
		sf::Text text;
		text.setFont(m_font);
		text.setFillColor(color);
		text.setCharacterSize(50);
		text.setString("Press ENTER to start");

		const sf::FloatRect bounds(text.getGlobalBounds());
		text.setPosition(m_x + m_width*0.5f - bounds.width * 0.5f, m_y + bounds.height*0.75f);

		target.draw(text);
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

	const std::size_t getTypedSize()
	{
		return getTyped().size();
	}

private:
	sf::Font m_font;
	InputZone m_input_zone;
};

