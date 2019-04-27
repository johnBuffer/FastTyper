#pragma once
#include <SFML/Graphics.hpp>
#include <transition.hpp>
#include "word.hpp"
#include "rounded_rectangle.hpp"

class InputZone : public sf::Drawable
{
public:
	InputZone(float width, float height, float x, float y)
		: m_width(width)
		, m_height(height)
		, m_x(x)
		, m_y(y)
		, m_typed(width*0.5f, 20.0f)
	{}

	void init(uint32_t character_size, const sf::Text& text)
	{
		setFont(*text.getFont());
		m_typed.init(character_size, text);
	}

	void addChar(char c)
	{
		const float offset_y(-24.0f);
		m_typed.add(c, offset_y);
		m_typed.center(m_width);
	}

	void pop()
	{
		m_typed.pop();
		m_typed.center(m_width);
	}

	void clear()
	{
		m_typed.clear();
	}

	void setFont(const sf::Font& font)
	{
		m_text.setFont(font);
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(14);

		m_text.setString("Input");
		m_text.setPosition(m_x + 18.0f, m_y + 8.0f);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::RectangleShape in_err(sf::Vector2f(m_width, m_height));
		in_err.setPosition(m_x, m_y);
		in_err.setFillColor(sf::Color::Red);
		//target.draw(in_err);

		RoundedRectangle word_in(m_width, m_height, 18.0f, m_x, m_y);
		word_in.setFillColor(sf::Color(80.0f, 80.0f, 80.0f));
		target.draw(word_in);

		sf::RenderStates rs;
		rs.transform.translate(m_x, m_y);
		target.draw(m_typed, rs);
		
		target.draw(m_text);
	}

private:
	float m_width, m_height;
	float m_x, m_y;

	Word m_typed;

	sf::Text m_text;
};


