#pragma once

#include <SFML/Graphics.hpp>
#include <transition.hpp>
#include <iostream>
#include "theme.hpp"

class Letter : public sf::Drawable
{
public:
	enum LetterState
	{
		Ok,
		Wrong,
		Skipped,
		Corrected,
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
		, m_g(255.0f)
		, m_b(255.0f)
	{
		setColor(Theme<>::LetterUnknown);
		m_text.setPosition(position);
		m_text.setString(c);
		m_y.setSpeed(3.0f);
		m_r.setSpeed(3.0f);
		m_g.setSpeed(3.0f);
		m_b.setSpeed(3.0f);

		m_advance = text.getFont()->getGlyph(c, text.getCharacterSize(), false).advance;
	}

	float getAdvance() const
	{
		return m_advance;
	}

	int32_t getLine() const
	{
		return m_line;
	}

	bool check(char c)
	{
		if (c == m_char)
		{
			if (m_state == LetterState::Unknown)
				setState(LetterState::Ok);
			else
				setState(LetterState::Corrected);
			return true;
		}
		
		setState(LetterState::Wrong);
		return false;
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
			setColor(Theme<>::LetterOk);
			break;
		case Letter::Wrong:
			setColor(Theme<>::LetterWrong);
			break;
		case Letter::Corrected:
			setColor(Theme<>::LetterCorrected);
			break;
		case Letter::Unknown:
			setColor(Theme<>::LetterUnknown);
			break;
		case Letter::Skipped:
			setColor(Theme<>::LetterSkipped);
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		m_text.setColor(sf::Color(m_r, m_g, m_b));
		m_text.setPosition(m_x, m_y);
		target.draw(m_text, states);
	}

	char getChar() const
	{
		return m_char;
	}

	float getX() const
	{
		return m_x;
	}

	float getY() const
	{
		return m_y;
	}

	const LetterState getState() const
	{
		return m_state;
	}

private:
	const char       m_char;
	mutable sf::Text m_text;
	int32_t          m_line;
	LetterState      m_state;
	float            m_advance;

	float m_x;
	trn::Transition<float> m_y;
	trn::Transition<float> m_r;
	trn::Transition<float> m_g;
	trn::Transition<float> m_b;

	void setColor(const sf::Color& color)
	{
		m_r = color.r;
		m_g = color.g;
		m_b = color.b;
	}
};
