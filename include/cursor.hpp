#pragma once
#include "transition.hpp"
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "theme.hpp"
#include "rounded_rectangle.hpp"
#include "letter.hpp"
#include "rectangle.hpp"


class Cursor : public sf::Drawable, public Rectangle
{
public:
	Cursor(float x, float y, float height)
		: m_width(0.0f)
		, m_height(height)
		, m_x(x)
		, m_y(y)
		, m_progress(0.0f)
		, m_word_length(0)
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
		m_text.setFillColor(Theme<>::LetterUnknown);
	}

	void setState(float x, float width)
	{
		m_x = x;
		m_width = width;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates rs) const override
	{
		const float width(m_width + 12.0f);
		const float x(m_x - 8.0f);
		const float y(m_y + 1.0f);
		const float progress_margin(6.0f);
		const float progress_max_width(width - 2.0f*progress_margin);
		const float progress_x(x + progress_margin);
		const float progress_y(y - 4.0f);
		const float text_progress_y(progress_y - 18.0f);

		RoundedRectangle cursor(width, 64, 8.0f, x, y - 8.0f);
		cursor.setFillColor(Theme<>::Color3);

		const float progress_bar_radius(4.0f);
		RoundedRectangle progress_bar(m_progress * 0.01f * progress_max_width, 8.0f, progress_bar_radius, progress_x, progress_y);
		RoundedRectangle progress_bar_back(progress_max_width, 8.0f, progress_bar_radius, progress_x, progress_y);
		progress_bar.setFillColor(Theme<>::LetterUnknown);
		progress_bar_back.setFillColor(Theme<>::Color4);

		target.draw(cursor);
		target.draw(progress_bar_back);

		if (m_progress > 2.0f*progress_bar_radius)
			target.draw(progress_bar);

		float progress = m_progress;
		m_text.setString(toString(progress, 0) + '%');
		m_text.setPosition(progress_x, text_progress_y);
		target.draw(m_text);
	}

	void drawProgress(sf::RenderTarget& target) const
	{
		const float width(m_width + 12.0f);
		const float x(m_x - 8.0f);
		const float y(m_y);

		const float progress_margin(8.0f);
		const float progress_max_width(width - 2.0f*progress_margin);
		const float progress_x(x + progress_margin);
		const float progress_y(y - 4.0f);
		const float text_progress_y(progress_y - 18.0f);

		RoundedRectangle progress_bar(m_progress * 0.01f * progress_max_width, 8.0f, 4.0f, progress_x, progress_y);

		progress_bar.setFillColor(Theme<>::LetterUnknown);
		if (m_progress > 0.1f) {
			target.draw(progress_bar);
		}
	}

	void setWordLenght(uint32_t length)
	{
		m_word_length = length;
		m_current_letter = 0;
		updateProgress();
	}

	void addChar()
	{
		++m_current_letter;
		updateProgress();
	}

	void prevLetter()
	{
		--m_current_letter;
		updateProgress();
	}

	float getX() const
	{
		return m_x;
	}

private:
	trn::Transition<float> m_width;
	float m_height;
	trn::Transition<float> m_x;
	float m_y;
	trn::Transition<float> m_progress;

	uint32_t m_word_length;
	uint32_t m_current_letter;

	mutable sf::Text m_text;

	void updateProgress()
	{
		const float progress_ratio(m_current_letter / float(m_word_length));
		m_progress = 100.0f * progress_ratio;
	}
};
