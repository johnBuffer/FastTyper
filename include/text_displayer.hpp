#pragma once

#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "letter.hpp"
#include "rounded_rectangle.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"
#include "challenge_status.hpp"
#include "PopUpLabel.hpp"
#include <list>
#include "font_dependant.hpp"

#include <iostream>

class TextDisplayer : public Rectangle, public sf::Drawable, public FontDependant
{
public:
	TextDisplayer(float width, float height, float x, float y)
		: Rectangle(width, height, x, y)
		, FontDependant()
		, m_space_y(0.0f)
		, m_margin(50.0f)
		, m_text_start_y(0.0f)
		, m_current_line(-1)
		, m_lines_to_display(2)
		, m_cursor(0.0f, y, 16.0f)
		, m_current_word(0)
		, m_current_char(0)
		, m_combo(0)
	{}

	void setBackgroundColor(const sf::Color& color)
	{
		m_background_color = color;
		m_background_color_transp = color;
		m_background_color_transp.a = 0.0f;
	}

	void setFont(const sf::Font& font, uint32_t char_size) override
	{
		FontDependant::setFont(font, char_size);

		m_space_y = font.getLineSpacing(m_char_size) + 8;
		m_cursor.setFont(font);

		m_text_start_y = m_y + (m_lines_to_display + 2) * m_space_y;
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
		if (m_current_line == -1) {
			m_cursor.setWordLenght(getCurrentWord().length);
			m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().getWordWidth(m_letters));
		}

		m_current_line += 1;
		for (Letter& letter : m_letters)
		{
			int32_t line(letter.getLine());
			if (line == m_current_line - 1)
			{
				letter.setY(-100.0f);
			}
			else if (line >= m_current_line && line < (m_current_line + m_lines_to_display))
			{
				int32_t i(m_current_line - line);
				letter.setY(m_y - i * m_space_y);
			}
		}
	}

	Letter& getCurrentLetter()
	{
		const WordInfo& current_word(getCurrentWord());
		uint32_t max_index(current_word.start_index + current_word.length - 1);
		return m_letters[std::min(m_current_char, max_index)];
	}

	Letter& getLetterAt(uint32_t i)
	{
		return m_letters[i];
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
		return m_letters;
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
		for (const PopUpLabel& pul : m_popups)
		{
			target.draw(pul);
		}

		if (m_current_line > -1) {
			target.draw(m_cursor);
		}

		const float display_height(m_space_y * m_lines_to_display);
		RoundedRectangle text_zone(m_width - m_margin, display_height, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(Theme<>::Color2);
		target.draw(text_zone);

		const float bottom_disp(m_y + display_height);
		const float bottom_text(m_text_start_y + m_space_y);
		const float mid((bottom_disp + bottom_text) * 0.5f);

		for (const Letter& letter : m_letters) {
			if (letter.getY() > m_y * 0.5f && letter.getY() < mid - m_space_y) {
				target.draw(letter);
			}
		}

		sf::VertexArray va_down(sf::Quads, 4);
		va_down[0].position = sf::Vector2f(0.0f   , bottom_disp);
		va_down[1].position = sf::Vector2f(m_width, bottom_disp);
		va_down[2].position = sf::Vector2f(m_width, mid);
		va_down[3].position = sf::Vector2f(0.0f   , mid);

		va_down[0].color = m_background_color_transp;
		va_down[1].color = m_background_color_transp;
		va_down[2].color = m_background_color;
		va_down[3].color = m_background_color;

		target.draw(va_down);

		sf::VertexArray va_up(sf::Quads, 4);
		va_up[0].position = sf::Vector2f(0.0f, m_y);
		va_up[1].position = sf::Vector2f(m_width, m_y);
		va_up[2].position = sf::Vector2f(m_width, m_y*0.5f);
		va_up[3].position = sf::Vector2f(0.0f, m_y*0.5f);

		va_up[0].color = m_background_color_transp;
		va_up[1].color = m_background_color_transp;
		va_up[2].color = m_background_color;
		va_up[3].color = m_background_color;

		target.draw(va_up);
	}

	void drawBloom(sf::RenderTarget& target, sf::RenderStates states) const
	{
		m_cursor.drawProgress(target);

		RoundedRectangle text_zone(m_width - m_margin, m_space_y * m_lines_to_display, 12.0f, m_margin*0.5f, m_y);
		text_zone.setFillColor(sf::Color::Black);
		target.draw(text_zone);

		for (const Letter& letter : m_letters) {
			if (letter.getY() > -m_space_y && letter.getY() < m_text_start_y) {
				if (letter.getState() == Letter::Wrong) {
					target.draw(letter);
				}
			}
		}
	}

	WordInfo& getCurrentWord()
	{
		return m_words[m_current_word];
	}

	const std::string& getNextword() const
	{
		const uint32_t word_count(m_words.size());
		const uint32_t next_word_index((m_current_word + 1) % word_count);
		return m_words[next_word_index].string;
	}

	const WordInfo& getCurrentWord() const
	{
		return m_words[m_current_word];
	}

	void nextWord(WordInfo::WordStatus word_status)
	{
		getCurrentWord().skipRest(m_letters);
		++m_current_word;
		m_current_char = getCurrentWord().start_index;
		if (getCurrentWord().first_of_line) {
			nextLine();
		}
		
		m_cursor.setWordLenght(getCurrentWord().length);
		m_cursor.setState(getCurrentLetter().getX(), getCurrentWord().getWordWidth(m_letters));

		addPopup(word_status);
	}

	void addPopup(WordInfo::WordStatus status)
	{
		const float x(m_cursor.getX());

		switch (status)
		{
		case WordInfo::Correct:
			m_combo = 0;
			m_popups.emplace_back("GOOD", x, m_y - m_height, m_font);
			break;
		case WordInfo::Perfect:
		{
			++m_combo;
			bool dual(false);
			std::string str("PERFECT");
			if (m_combo > 2)
			{
				dual = true;
				str = "x" + toString(m_combo, 0) + "\n" + str;
			}

			m_popups.emplace_back(str, x, m_y - m_height, m_font, dual);
			break;
		}
		case WordInfo::Wrong:
			m_combo = 0;
			m_popups.emplace_back("ERROR", x, m_y - m_height, m_font);
			break;
		default:
			break;
		}
	}

	void reset()
	{
		m_current_line = -1;
		m_current_char = 0;
		m_current_word = 0;
		m_letters.clear();
		m_words.clear();
		m_combo = 0;
	}

	void initialize(const std::vector<std::string>& words)
	{
		reset();

		const float space_x(16.0f);

		Line line(m_margin, 0.0f);
		line.pos.x = m_margin;
		line.pos.y = m_text_start_y;

		for (const std::string& word : words)
		{
			m_words.emplace_back(word, m_letters.size());
			m_words.back().first_of_line = wordToLetters(line, word, getFont(), m_char_size);
			line.pos.x += space_x;
		}
	}

private:
	int32_t m_current_line;

	float m_space_y;
	float m_margin;
	float m_text_start_y;

	uint32_t m_lines_to_display;

	Cursor m_cursor;
	uint32_t m_current_word;
	uint32_t m_current_char;

	std::vector<WordInfo> m_words;
	std::vector<Letter>   m_letters;

	sf::Color m_background_color;
	sf::Color m_background_color_transp;

	std::list<PopUpLabel> m_popups;
	uint32_t m_combo;

	bool wordToLetters(Line& line, const std::string& word, const sf::Font& font, uint32_t char_size)
	{
		bool new_line(false);
		const uint32_t start_index(m_letters.size());
		uint32_t current_index(0);
		float width(0.0f);

		for (const char c : word)
		{
			m_letters.emplace_back(c, line.pos, line.line_count, font, char_size);
			Letter& current_letter(m_letters.back());

			const float advance(current_letter.getAdvance());
			line.pos.x += advance;
			width += advance;

			// If we hit the max width
			if (line.pos.x > m_width - line.margin)
			{
				// New line
				new_line = true;
				// Increase y and reset x
				line.newLine();
				// Update letters
				for (uint32_t i(0); i < current_index + 1; ++i)
				{
					uint32_t letter_index(start_index + i);
					Letter& letter_to_update(m_letters[letter_index]);
					letter_to_update.setX(line.pos.x);
					letter_to_update.addLine();
					line.pos.x += letter_to_update.getAdvance();
				}
			}

			++current_index;
		}

		const float min_width(65.0f);
		if (width < min_width)
		{
			line.pos.x += min_width - width;
		}

		return new_line;
	}
};
