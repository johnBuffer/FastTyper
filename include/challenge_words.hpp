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
#include "challenge_status.hpp"

class ChallengeWords : public Rectangle
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

	void nextLine();
	void render(sf::RenderTarget& target);
	void renderBloom(sf::RenderTarget& target);
	void addChar(uint32_t c);
	void removeChar();
	void update();
	void nextWord();
	void use(const Replay& replay);

	Letter& getLetter();
	WordInfo& getCurrentWord();

	void  exportReplay() const;
	float getProgress() const;

	const WordInfo& getCurrentWord() const;

	static void init(const std::string& dico_path);

private:
	float          m_text_y;
	float          m_space_y;
	sf::Font       m_font;
	uint32_t       m_char_size;

	int32_t        m_current_line;
	uint32_t       m_lines_to_display;
	
	std::vector<WordInfo> m_words;
	std::vector<Letter>   m_letters;
	TypingZone            m_input;

	Replay m_recorder;

	ChallengeStatus m_status;

	Blur m_blur;
	sf::RenderTexture m_blur_texture;

	StatsDisplayer m_stats;

	Cursor m_cursor;

	static std::vector<std::string> s_words_set;
	
	void wordToLetters(Line& line, const std::string& word, const sf::Text& text);
	void initwords(const sf::Text& text);
	uint32_t getCurrentCharInWord() const;
	void reset();
};
