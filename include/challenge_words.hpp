#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <transition.hpp>
#include <dynamic_blur.hpp>
#include "letter.hpp"
#include "line.hpp"
#include "wordinfo.hpp"
#include "cursor.hpp"
#include "typing_zone.hpp"
#include "metrics.hpp"
#include "rectangle.hpp"
#include "replay.hpp"
#include "stats_display.hpp"

class ChallengeWords : public Rectangle
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void nextLine();
	void render(sf::RenderTarget& target);
	void renderBloom(sf::RenderTarget& target);
	void addChar(uint32_t c);
	void removeChar();

	WordInfo& getCurrentWord()
	{
		return m_words[m_current_word];
	}

	const WordInfo& getCurrentWord() const
	{
		return m_words[m_current_word];
	}

	Letter& getLetter()
	{
		return m_letters[m_current_char];
	}

	float getAccuracy() const
	{
		if (m_clock.getElapsedTime().asSeconds() < 0.5f)
			return 0.0f;

		const float entries(m_entry_count);
		const float errors(m_error_count);
		const float accuracy((entries - errors) / entries);

		return std::max(0.0f, accuracy);
	}

	float getWPM() const
	{
		if (m_clock.getElapsedTime().asSeconds() < 0.5f)
			return 0.0f;

		const float entries(m_entry_count);
		const float errors(m_error_count);
		const float time(m_clock.getElapsedTime().asMilliseconds() * 0.001f / 60.0f);
		const float wpm((entries * 0.2f - errors) / time);

		return std::max(0.0f, wpm);
	}

	uint32_t getCurrentChellengeTime() const
	{
		return m_clock.getElapsedTime().asMilliseconds();
	}

	float getProgress() const;

	void update();

	static void init(const std::string& dico_path);

private:
	float          m_text_y;
	float          m_space_y;
	sf::Font       m_font;
	uint32_t       m_char_size;

	int32_t        m_current_line;
	uint32_t       m_lines_to_display;
	uint32_t       m_current_char;
	uint32_t       m_current_word;
	std::string    m_typed;
	
	std::vector<WordInfo> m_words;
	std::vector<Letter>   m_letters;
	TypingZone            m_input;

	Replay m_recorder;

	Blur m_blur;
	sf::RenderTexture m_blur_texture;

	bool m_started;
	sf::Clock m_clock;
	uint32_t m_entry_count;
	uint32_t m_error_count;

	StatsDisplayer m_stats;

	Cursor m_cursor;

	static std::vector<std::string> s_words_set;
	
	void wordToLetters(Line& line, const std::string& word, const sf::Text& text);
	void initwords(const sf::Text& text);
	uint32_t getCurrentCharInWord() const;
};
