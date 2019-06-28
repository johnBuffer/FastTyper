#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "letter.hpp"
#include "rounded_rectangle.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"
#include "challenge_status.hpp"
#include "pop_up_label.hpp"
#include <list>
#include "font_dependant.hpp"
#include "line_roll.hpp"

#include <iostream>

class TextDisplayer : public Rectangle, public sf::Drawable, public FontDependant
{
public:
	TextDisplayer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, FontDependant()
		, m_margin(50.0f)
		, m_cursor(0.0f, y, 16.0f)
		, m_current_word(0)
		, m_current_char(0)
		, m_combo(0)
		, m_roll(width, x, y, 2)
	{}

	void setBackgroundColor(const sf::Color& color)
	{
		m_roll.setBackgroundColor(color);
	}

	void setFont(const sf::Font& font, uint32_t char_size) override
	{
		FontDependant::setFont(font, char_size);

		m_cursor.setFont(font);
		m_roll.setFont(font, char_size);
		m_height = m_roll.getHeight();
	}

	void update()
	{
		for (PopUpLabel& pul : m_popups)
		{
			pul.update();
		}

		m_popups.remove_if([&](const PopUpLabel& p) {return p.done(); });
	}

	void nextLine()
	{
		if (m_roll.getCurrentLine() == -1) {
			m_cursor.setWordLenght(getCurrentWord().length);
			m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().width);
		}

		m_roll.roll();
	}

	Letter& getCurrentLetter()
	{
		const WordInfo& current_word(getCurrentWord());
		const uint32_t max_index(current_word.start_index + current_word.length - 1);
		return m_roll.getLetterAt(std::min(m_current_char, max_index));
	}

	uint32_t getCurrentCharInWord() const
	{
		return m_current_char - getCurrentWord().start_index;
	}

	float getProgress() const
	{
		return 100.0f * (getCurrentCharInWord() / float(getCurrentWord().length));
	}

	uint32_t getCurrentCharIndex() const
	{
		return m_current_char;
	}

	std::vector<Letter>& getLetters()
	{
		return m_roll.getLetters();
	}

	void nextChar(Letter::LetterState state)
	{
		if (state != Letter::Outside) {
			getCurrentLetter().setState(state);
			++m_current_char;
			m_cursor.addChar();
		}
	}

	void prevChar()
	{
		--m_current_char;
		m_cursor.prevLetter();
		getCurrentLetter().setState(Letter::Skipped);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (const PopUpLabel& pul : m_popups) {
			target.draw(pul);
		}

		if (m_roll.getCurrentLine() > -1) {
			target.draw(m_cursor);
		}

		RoundedRectangle text_zone(m_width - m_margin, m_height, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(Theme<>::Color2);
		target.draw(text_zone);

		m_roll.draw(target, states);
	}

	void drawBloom(sf::RenderTarget& target, sf::RenderStates states) const
	{
		m_cursor.drawProgress(target);

		RoundedRectangle text_zone(m_width - m_margin, m_height, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(sf::Color::Black);
		target.draw(text_zone);

		m_roll.drawBloom(target, states);
	}

	WordInfo& getCurrentWord()
	{
		return m_words[m_current_word];
	}

	const std::string& getNextword() const
	{
		const std::size_t word_count(m_words.size());
		const std::size_t next_word_index((m_current_word + 1) % word_count);
		return m_words[next_word_index].string;
	}

	const WordInfo& getCurrentWord() const
	{
		return m_words[m_current_word];
	}

	void nextWord(WordInfo::WordStatus word_status)
	{
		getCurrentWord().skipRest(m_roll.getLetters());
		++m_current_word;
		m_current_char = getCurrentWord().start_index;
		if (getCurrentWord().first_of_line) {
			nextLine();
		}
		
		m_cursor.setWordLenght(getCurrentWord().length);
		m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().width);

		addPopup(word_status);
	}

	void addPopup(WordInfo::WordStatus status)
	{
		const float x(m_cursor.getX());

		switch (status)
		{
		case WordInfo::Correct:
			m_combo = 0;
			m_popups.emplace_back("GOOD", x, m_y, getFont());
			break;
		case WordInfo::Perfect:
		{
			++m_combo;
			bool dual(false);
			std::string str("PERFECT");
			if (m_combo > 2) {
				dual = true;
				str = "x" + toString(m_combo, 0) + "\n" + str;
			}

			m_popups.emplace_back(str, x, m_y, getFont(), dual);
			break;
		}
		case WordInfo::Wrong:
			m_combo = 0;
			m_popups.emplace_back("ERROR", x, m_y, getFont());
			break;
		default:
			break;
		}
	}

	void reset()
	{
		m_roll.clear();
		m_current_char = 0;
		m_current_word = 0;
		m_words.clear();
		m_combo = 0;
	}

	void initialize(const std::vector<std::string>& words)
	{
		reset();

		m_roll.generateLetters(m_margin, words, m_words);
	}

private:
	float m_margin;

	Cursor m_cursor;
	uint32_t m_current_word;
	uint32_t m_current_char;

	std::vector<WordInfo> m_words;

	std::list<PopUpLabel> m_popups;
	uint32_t m_combo;

	LineRoll m_roll;
};
