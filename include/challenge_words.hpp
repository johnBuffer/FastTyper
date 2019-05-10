#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <dynamic_blur.hpp>
#include "line.hpp"
#include "wordinfo.hpp"
#include "typing_zone.hpp"
#include "metrics.hpp"
#include "replay.hpp"
#include "stats_display.hpp"
#include "challenge_status.hpp"
#include "text_displayer.hpp"
#include "circle_clock.hpp"

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
	void use(const ChallengeRecorder& replay);
	void exportReplay() const;
	void start();
	void setBackgroundColor(const sf::Color& color);

	static void init(const std::string& dico_path);

private:
	sf::Font m_font;
	TypingZone m_input;
	ChallengeRecorder m_recorder;
	ChallengeStatus m_status;
	TextDisplayer m_text_displayer;
	StatsDisplayer m_stats;
	
	const float m_duration;
	CircleClock m_timer;

	Blur m_blur;
	sf::RenderTexture m_blur_texture;
	static std::vector<std::string> s_words_set;

	// Private methods
	void initwords();
	void reset();
	void newChallenge();
};
