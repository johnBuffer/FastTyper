#include "challenge_words.hpp"
#include "circle_clock.hpp"
#include <iostream>
#include "rounded_rectangle.hpp"

std::vector<std::string> ChallengeWords::s_words_set;

ChallengeWords::ChallengeWords(uint32_t width, uint32_t height)
	: Rectangle(width, height, 0.0f, 0.0f)
	, m_text_y(320.0f)
	, m_current_line(-1)
	, m_lines_to_display(2)
	, m_char_size(40)
	, m_stats(width, 50.0f, 0.0f, 500.0f)
	, m_input(800.0f, 120.0f, (width - 800.0f)*0.5f, 700)
	, m_cursor(0.0f, m_text_y, 16.0f)
	, m_blur(width, height, 1.0f)
{
	m_blur_texture.create(width, height);
	m_font.loadFromFile("font_med.ttf");
	m_cursor.setFont(m_font);
	
	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(Theme<>::LetterUnknown);
	text.setCharacterSize(m_char_size);
	m_space_y = m_font.getLineSpacing(m_char_size) + 8;

	m_stats.setFont(m_font);

	m_input.init(64, text);
	initwords(text);
	m_cursor.setState(getLetter().getX(), getCurrentWord().getWordWidth(m_letters));
}

void ChallengeWords::nextLine()
{
	m_current_line += 1;
	for (Letter& letter : m_letters)
	{
		int32_t line(letter.getLine());
		if (line == m_current_line-1)
		{
			letter.setY(-100.0f);
		}
		else if (line >= m_current_line && line < (m_current_line + m_lines_to_display))
		{
			int32_t i(m_current_line - line);
			letter.setY(m_text_y - i*m_space_y);
		}
	}
}

void ChallengeWords::render(sf::RenderTarget& target)
{
	m_blur_texture.clear();

	m_cursor.drawProgress(m_blur_texture);
	target.draw(m_cursor);
	
	RoundedRectangle text_zone(m_width - 50.0f, m_space_y * m_lines_to_display, 12.0f, 25.0f, m_text_y);
	text_zone.setFillColor(Theme<>::Color2);
	target.draw(text_zone);
	text_zone.setFillColor(sf::Color::Black);
	m_blur_texture.draw(text_zone);

	for (const Letter& letter : m_letters) {
		if (letter.getY() > -m_space_y && letter.getY() < 400.0f) {
			target.draw(letter);
			if (letter.getState() == Letter::Wrong) {
				m_blur_texture.draw(letter);
			}
		}
	}

	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(50);
	const float clock_y(150.0f);

	float ratio(1.0f);
	if (m_status.started) {
		ratio = 1.0f - 0.001f * m_status.getElapsedMilliseconds() / 60.0f;
		text.setString(toString(60 - m_status.getElapsedSeconds(), 0)+'s');
	} else {
		text.setString("60s");
	}

	text.setPosition((m_width - text.getGlobalBounds().width) * 0.5f, clock_y - 35.0f);
	target.draw(text);

	target.draw(m_stats);
	m_stats.drawBloom(m_blur_texture);

	const CircleClock clock(80.0f, 800.0f, clock_y, ratio);
	CircleClock clock_back(80.0f, 800.0f, clock_y, 1.0f);
	clock_back.setFillColor(Theme<>::LetterSkipped);
	target.draw(clock);
	target.draw(clock_back);
	m_blur_texture.draw(clock);

	target.draw(m_input);
}

void ChallengeWords::renderBloom(sf::RenderTarget& target)
{
	m_blur_texture.display();
	target.draw(m_blur.apply(m_blur_texture.getTexture(), 1.0f), sf::BlendAdd);
}

void ChallengeWords::addChar(uint32_t unicode)
{
	char c(static_cast<char>(unicode));
	if (c > 'z' || c < '\'') {
		return;
	}
	
	m_input.getInput().addChar(c);
	bool is_ok(getLetter().check(c) && m_status.typed.size() < getCurrentWord().length);
	m_status.addChar(c, is_ok, getCurrentWord().length);

	m_cursor.setProgress(getProgress());
	m_recorder.addChar(unicode, m_status.getElapsedMilliseconds());
}

void ChallengeWords::removeChar()
{
	m_recorder.removeChar(m_status.getElapsedMilliseconds());

	uint32_t size(m_status.typed.size());
	if (!size)
		return;

	--size;
	m_input.getInput().pop();
	if (size == getCurrentCharInWord()) {
		getLetter().setState(Letter::Skipped);
	}
	else if (m_status.current_char && size < getCurrentCharInWord()) {
		--m_status.current_char;
		getLetter().setState(Letter::Skipped);
	}

	m_status.pop();
	m_cursor.setProgress(getProgress());
}

WordInfo& ChallengeWords::getCurrentWord()
{
	return m_words[m_status.current_word];
}

const WordInfo& ChallengeWords::getCurrentWord() const
{
	return m_words[m_status.current_word];
}

Letter& ChallengeWords::getLetter()
{
	return m_letters[m_status.current_char];
}

void ChallengeWords::use(const Replay& replay)
{
	m_letters.clear();
	m_words.clear();

	sf::Text text;
	text.setFont(m_font);
	text.setFillColor(Theme<>::LetterUnknown);
	text.setCharacterSize(m_char_size);

	const auto& words(replay.getWords());
	
	const float margin(50.0f);
	const float space_x(16.0f);

	Line line(margin, 0.0f);
	line.pos.y = 900.0f;

	for (const std::string& word : words)
	{
		m_words.emplace_back(word, m_letters.size());
		wordToLetters(line, word, text);
		line.pos.x += space_x;
	}

	reset();
}

void ChallengeWords::exportReplay() const
{
	if (!m_status.started)
	{
		m_recorder.toFile();
	}
}

float ChallengeWords::getProgress() const
{
	return 100.0f * (m_status.typed.size() / float(getCurrentWord().length));
}

void ChallengeWords::update()
{
	const uint32_t millis_to_minute(60000);
	uint32_t current_time(m_status.getElapsedMilliseconds());
	if (m_status.started)
	{
		if (current_time <millis_to_minute)
		{
			m_stats.update(m_status);
		}
		else
		{
			m_status.started = false;
		}
	}
}

void ChallengeWords::nextWord()
{
	const Letter& last_letter(getLetter());
	m_input.getInput().clear();
	// The number of char skipped from current word
	m_status.nextWord(getCurrentWord().skipRest(m_letters));
	// Jump to next word's first char
	m_status.current_char = getCurrentWord().start_index;
	// Cursor update
	m_cursor.setState(getLetter().getX(), getCurrentWord().getWordWidth(m_letters));
	// Check for new line
	if (getLetter().getLine() != last_letter.getLine())
		nextLine();
	// Update cursor progress
	m_cursor.setProgress(getProgress());

	m_recorder.nextWord(m_status.getElapsedMilliseconds());
}

void ChallengeWords::init(const std::string& dico_path)
{
	s_words_set = { "the", "be", "of", "and", "a", "to", "in", "he", "have", "it", "that", "for", "they", "I", "with", "as", "not", "on", "she", "at", "by", "this", "we", "you", "do", "but", "from", "or", "which", "one", "would", "all", "will", "there", "say", "who", "make", "when", "can", "more", "if", "no", "man", "out", "other", "so", "what", "time", "up", "go", "about", "than", "into", "could", "state", "only", "new", "year", "some", "take", "come", "these", "know", "see", "use", "get", "like", "then", "first", "any", "work", "now", "may", "such", "give", "over", "think", "most", "even", "find", "day", "also", "after", "way", "many", "must", "look", "before", "great", "back", "through", "long", "where", "much", "should", "well", "people", "down", "own", "just", "because", "good", "each", "those", "feel", "seem", "how", "high", "too", "place", "little", "world", "very", "still", "nation", "hand", "old", "life", "tell", "write", "become", "here", "show", "house", "both", "between", "need", "mean", "call", "develop", "under", "last", "right", "move", "thing", "general", "school", "never", "same", "another", "begin", "while", "number", "part", "turn", "real", "leave", "might", "want", "point", "form", "off", "child", "few", "small", "since", "against", "ask", "late", "home", "interest", "large", "person", "end", "open", "public", "follow", "during", "present", "without", "again", "hold", "govern", "around", "possible", "head", "consider", "word", "program", "problem", "however", "lead", "system", "set", "order", "eye", "plan", "run", "keep", "face", "fact", "group", "play", "stand", "increase", "early", "course", "change", "help", "line" };
}

void ChallengeWords::wordToLetters(Line& line, const std::string& word, const sf::Text& text)
{
	const uint32_t start_index(m_letters.size());
	uint32_t current_index(0);
	for (const char c : word)
	{
		m_letters.emplace_back(c, line.pos, line.line_count, text);
		Letter& current_letter(m_letters.back());
		line.pos.x += current_letter.getAdvance();

		// If we hit the max width
		if (line.pos.x > m_width - line.margin)
		{
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
}

void ChallengeWords::initwords(const sf::Text& text)
{
	const float margin(50.0f);
	const float space_x(16.0f);

	Line line(margin, 0.0f);
	line.pos.y = 900.0f;

	const uint32_t word_count(300);
	uint32_t size(s_words_set.size());

	for (uint32_t i(word_count); --i;)
	{
		uint32_t index(rand() % size);
		const std::string word(s_words_set[index]);

		m_recorder.addWord(word);

		m_words.emplace_back(word, m_letters.size());

		wordToLetters(line, word, text);
		line.pos.x += space_x;
	}

	nextLine();
}

uint32_t ChallengeWords::getCurrentCharInWord() const
{
	return m_status.current_char - getCurrentWord().start_index;
}

void ChallengeWords::reset()
{
	m_status.reset();

	m_recorder.clear();
	m_current_line = -1;
	nextLine();
}
