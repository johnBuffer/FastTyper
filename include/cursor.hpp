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
		const float width(m_width + 12.0f);
		const float x(m_x - 8.0f);
		const float y(m_y - m_height - 8.0f);
		const float progress_y(y - 16.0f);
		const float text_progress_y(progress_y - 16.0f);

		sf::RectangleShape cursor0(sf::Vector2f(width, 2.0f));
		sf::RectangleShape cursor1(sf::Vector2f(2.0f, 8.0f));
		sf::RectangleShape cursor2(sf::Vector2f(2.0f, 8.0f));

		sf::RectangleShape progress_bar(sf::Vector2f(m_progress * 0.01f * width, 4.0f));
		progress_bar.setFillColor(sf::Color(36, 142, 230));
		progress_bar.setPosition(x, progress_y);

		cursor0.setPosition(x        , y - 8.0f);
		cursor1.setPosition(x        , y - 8.0f);
		cursor2.setPosition(x + width, y - 8.0f);

		target.draw(cursor0);
		target.draw(cursor1);
		target.draw(cursor2);
		target.draw(progress_bar);

		float progress = m_progress;
		m_text.setString(toString(progress, 0) + '%');
		m_text.setPosition(x, text_progress_y);
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
