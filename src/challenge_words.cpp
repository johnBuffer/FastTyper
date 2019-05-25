#include "challenge_words.hpp"
#include "circle_clock.hpp"
#include <iostream>
#include "rounded_rectangle.hpp"

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height)
	: Rectangle(width, height, 0.0f, 0.0f)
	, m_text_displayer(width, 0.0f, 0.0f, 320.0f)
	, m_stats(width, 50.0f, 0.0f, 500.0f)
	, m_input(800.0f, 120.0f, (width - 800.0f)*0.5f, 700)
	, m_blur(width, height, 1.0f)
	, m_duration(60.0f)
	, m_timer(100.0f, width*0.5f, 150.0f, m_duration)
	, m_results(m_width * 0.5f, -200.0f)
{
	m_blur_texture.create(width, height);
	m_font.loadFromFile("font_med.ttf");
	
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(Theme<>::LetterUnknown);

	m_stats.setFont(m_font);
	m_text_displayer.setFont(m_font, 40);
	m_timer.setFont(m_font, 70);
	m_results.setFont(m_font);
	m_input.setFont(m_font, 64);
}

void ChallengeWords::render(sf::RenderTarget& target)
{
	m_blur_texture.clear();
	
	target.draw(m_text_displayer);
	target.draw(m_stats);
	target.draw(m_timer);
	target.draw(m_input);
	target.draw(m_results);

	if (!m_status.started) {
		const float t(0.001f * m_status.clock.getElapsedTime().asMilliseconds());
		const float c(127 + 128*sin(t)*sin(t));
		const sf::Color color(c, c,c);
		m_input.showInstruction(target, color);
		if (!m_recorder.isEmpty()) {
			showHelp(25.0f, 25.0f, m_font, target, m_recorder.isExported());
		}
	}
}

void ChallengeWords::renderBloom(sf::RenderTarget& target)
{
	sf::RenderStates rs;
	m_text_displayer.drawBloom(m_blur_texture, rs);
	m_stats.drawBloom(m_blur_texture);
	m_blur_texture.display();
	target.draw(m_blur.apply(m_blur_texture.getTexture(), 1.0f), sf::BlendAdd);
}

void ChallengeWords::addChar(uint32_t unicode)
{
	char c(getCharFromUnicode(unicode));
	if (isValidChar(c) && m_status.started) {
		m_input.getInput().addChar(c);
		Letter::LetterState letter_state = m_status.addChar(m_input.getTyped(), m_text_displayer.getCurrentLetter());
		m_text_displayer.nextChar(letter_state);
		m_recorder.addChar(unicode, m_status.getElapsedMilliseconds());

		if (!m_status.replay_mode) {
			m_word_stats.addStat(m_text_displayer.getCurrentWord().string, Letter::isCorrect(letter_state));
			m_word_stats.addStat(std::string(1, m_text_displayer.getCurrentLetter().getChar()), Letter::isCorrect(letter_state));
		}
	}
}

void ChallengeWords::removeChar()
{
	m_recorder.removeChar(m_status.getElapsedMilliseconds());

	uint32_t size(m_input.getTyped().size());
	if (size--) {
		m_input.getInput().pop();
		const uint32_t currentCharInWord(m_text_displayer.getCurrentCharInWord());
		if (m_text_displayer.getCurrentCharIndex() && size < currentCharInWord) {
			m_text_displayer.prevChar();
			m_status.pop();
		}
	}
}

void ChallengeWords::playRecord(const ChallengeRecorder& replay)
{
	reset();
	m_status.replay_mode = true;
	m_text_displayer.initialize(replay.getWords());
	m_text_displayer.nextLine();
}

void ChallengeWords::exportReplay() const
{
	if (!m_status.started) {
		m_recorder.toFile();
	}
}

void ChallengeWords::start()
{
	if (m_status.started) {
		stopChallenge();
	}

	newChallenge();
}

void ChallengeWords::setBackgroundColor(const sf::Color& color)
{
	m_text_displayer.setBackgroundColor(color);
}

void ChallengeWords::update()
{
	const uint32_t millis_to_second(1000);
	const uint32_t millis_to_minute(60 * millis_to_second);
	const uint32_t current_time(m_status.getElapsedMilliseconds());
	if (m_status.started)
	{
		if (current_time < m_duration * millis_to_second) {
			m_stats.update(m_status);
			m_text_displayer.update();
		} else {
			stopChallenge();
		}
	}
}

void ChallengeWords::nextWord()
{
	if (m_status.started) {
		const WordInfo::WordStatus word_status = m_status.nextWord(m_input.getTyped(), m_text_displayer.getNextword());
		m_input.getInput().clear();
		m_text_displayer.nextWord(word_status);
		m_recorder.nextWord(m_status.getElapsedMilliseconds());
	}
}

void ChallengeWords::init(const std::string& dico_path)
{
	s_words_set = { "the", "be", "of", "and", "a", "to", "in", "he", "have", "it", "that", "for", "they", "I", "with", "as", "not", "on", "she", "at", "by", "this", "we", "you", "do", "but", "from", "or", "which", "one", "would", "all", "will", "there", "say", "who", "make", "when", "can", "more", "if", "no", "man", "out", "other", "so", "what", "time", "up", "go", "about", "than", "into", "could", "state", "only", "new", "year", "some", "take", "come", "these", "know", "see", "use", "get", "like", "then", "first", "any", "work", "now", "may", "such", "give", "over", "think", "most", "even", "find", "day", "also", "after", "way", "many", "must", "look", "before", "great", "back", "through", "long", "where", "much", "should", "well", "people", "down", "own", "just", "because", "good", "each", "those", "feel", "seem", "how", "high", "too", "place", "little", "world", "very", "still", "nation", "hand", "old", "life", "tell", "write", "become", "here", "show", "house", "both", "between", "need", "mean", "call", "develop", "under", "last", "right", "move", "thing", "general", "school", "never", "same", "another", "begin", "while", "number", "part", "turn", "real", "leave", "might", "want", "point", "form", "off", "child", "few", "small", "since", "against", "ask", "late", "home", "interest", "large", "person", "end", "open", "public", "follow", "during", "present", "without", "again", "hold", "govern", "around", "possible", "head", "consider", "word", "program", "problem", "however", "lead", "system", "set", "order", "eye", "plan", "run", "keep", "face", "fact", "group", "play", "stand", "increase", "early", "course", "change", "help", "line" };
}

void ChallengeWords::initwords()
{
	const uint32_t word_count(300);
	std::vector<std::string> words;
	for (uint32_t i(word_count); --i;) {
		const std::string& word(getRandomElemFromVector(s_words_set));
		m_recorder.addWord(word);
		words.emplace_back(word);
	}

	m_text_displayer.initialize(words);
}

void ChallengeWords::reset()
{
	m_text_displayer.reset();
	m_status.reset();
	m_status.started = true;
	m_recorder.clear();
	m_timer.start();
	m_input.getInput().clear();
}

void ChallengeWords::newChallenge()
{
	reset();
	m_timer.setY(150.0f);
	m_results.setY(-200.0f);
	initwords();
	m_text_displayer.nextLine();
	m_status.current_word_str = m_text_displayer.getCurrentWord().string;

}

void ChallengeWords::stopChallenge()
{
	m_results.setValue(m_status.correct_word_count, m_status.perfect_word_count, m_status.total_word_count);
	m_status.started = false;
	m_timer.reset();

	m_input.getInput().clear();
	m_results.setY(50.0f);
	m_timer.setY(-200.0f);

	m_word_stats.toFile();
}

