#pragma once
#include "transition.hpp"
#include <SFML/Graphics.hpp>
#include "utils.hpp"

class Cursor : public sf::Drawable
{
public:
	Cursor(float x, float y, float height)
		: m_width(0.0f)
		, m_height(height)
		, m_x(x)
		, m_y(y)
		, m_progress(0.0f)
	{
		m_width.setSpeed(4.0f);
		m_x.setSpeed(4.0f);
		m_progress.setSpeed(10.0f);
	}

	void setFont(const sf::Font& font)
	{
		m_text.setFont(font);
		m_text.setCharacterSize(12);
		m_text.setPosition(m_x, m_y);
		m_text.setFillColor(sf::Color::White);
	}

	void setState(float x, float length)
	{
		m_x = x;
		m_width = length;
	}

	void setProgress(float progress)
	{
		m_progress = std::min(progress, 100.0f);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates rs) const override
	{
		sf::RectangleShape cursor0(sf::Vector2f(m_width + 12.0f, 2.0f));
		sf::RectangleShape cursor1(sf::Vector2f(2.0f, 16.0f));
		sf::RectangleShape cursor2(sf::Vector2f(2.0f, 16.0f));

		sf::RectangleShape progress_bar(sf::Vector2f(m_progress * 0.01f * (m_width + 12.0f), 4.0f));
		progress_bar.setFillColor(sf::Color(80, 95, 144));
		//progress_bar.setFillColor(sf::Color(67, 133, 245));
		progress_bar.setPosition(m_x - 8.0f, m_y - 32.0f);

		cursor0.setPosition(m_x - 8.0f          , m_y - m_height - 8.0f);
		cursor1.setPosition(m_x - 8.0f          , m_y - m_height - 8.0f);
		cursor2.setPosition(m_x + m_width + 4.0f, m_y - m_height - 8.0f);

		target.draw(cursor0);
		target.draw(cursor1);
		target.draw(cursor2);
		target.draw(progress_bar);

		float progress = m_progress;
		m_text.setString(toString(progress, 0) + '%');
		m_text.setPosition(m_x - 8.0f, m_y - 48.0f);
		target.draw(m_text);
	}

private:
	trn::Transition<float> m_width;
	float m_height;

	trn::Transition<float> m_x;
	float m_y;

	trn::Transition<float> m_progress;

	mutable sf::Text m_text;

};
