#pragma once

#include <SFML/Graphics.hpp>
#include <transition.hpp>
#include <iostream>

class Letter : public sf::Drawable
{
public:
	enum LetterState
	{
		Ok,
		Wrong,
		Unknown,
	};

	Letter(char c, const sf::Vector2f& position, uint32_t line, const sf::Text& text) 
		: m_char(c)
		, m_text(text)
		, m_line(line)
		, m_state(Letter::Unknown)
		, m_x(position.x)
		, m_y(position.y)
		, m_r(255.0f)
		, m_b(255.0f)
	{
		m_text.setPosition(position);
		m_text.setString(c);
		m_y.setSpeed(2.0f);
		m_bounds = m_text.getGlobalBounds();
	}

	const sf::FloatRect& getBounds() const
	{
		return m_bounds;
	}

	int32_t getLine() const
	{
		return m_line;
	}

	void setY(float y)
	{
		m_y = y;
	}

	void addY(float dy)
	{
		m_y += dy;
	}

	void setX(float x)
	{
		m_x = x;
	}

	void addLine()
	{
		++m_line;
	}

	void setState(LetterState state)
	{
		m_state = state;
		switch (state)
		{
		case Letter::Ok:
			m_r = 0.0f;
			m_b = 0.0f;
			break;
		case Letter::Wrong:
			m_r = 255.0f;
			m_b = 0.0f;
			break;
		case Letter::Unknown:
			m_r = 0.0f;
			m_b = 255.0f;
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		float r(m_r);
		float g(255.0f);
		float b(m_b);
		m_text.setColor(sf::Color(r, g, b));
		//std::cout << m_r << " " << m_b << std::endl;
		m_text.setPosition(m_x, m_y);
		target.draw(m_text, states);
	}

private:
	const char       m_char;
	mutable sf::Text m_text;
	int32_t          m_line;
	LetterState      m_state;
	sf::FloatRect    m_bounds;

	float m_x;
	trn::Transition<float> m_y;
	trn::Transition<float> m_r;
	trn::Transition<float> m_b;
};
