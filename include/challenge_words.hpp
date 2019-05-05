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
#include "text_displayer.hpp"

class ChallengeWords : public Rectangle
{
public:
	ChallengeWords(uint32_t width, uint32_t height);

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
	sf::Font m_font;
	std::vector<WordInfo> m_words;
	TypingZone m_input;
	Replay m_recorder;
	ChallengeStatus m_status;
	TextDisplayer m_text_displayer;
	StatsDisplayer m_stats;

	Blur m_blur;
	sf::RenderTexture m_blur_texture;
	static std::vector<std::string> s_words_set;

	// Private methods
	void initwords(const sf::Text& text);
	uint32_t getCurrentCharInWord() const;
	void reset();
};
