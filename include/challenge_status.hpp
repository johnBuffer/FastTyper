#pragma once

#include <SFML/System/Clock.hpp>
#include <stdint.h>
#include <string>

struct ChallengeStatus
{
	ChallengeStatus()
		: entry_no_error(0)
		, entry_count(0)
		, error_count(0)
		, correct_word_count(0)
		, perfect_word_count(0)
		, started(false)
		, current_word_perfect(true)
	{}

	void addChar(Letter::LetterState state)
	{
		++entry_count;
		switch (state)
		{
		case Letter::Ok:
			++entry_no_error;
			break;
		case Letter::Wrong:
			error();
			break;
		case Letter::Corrected:
			++entry_no_error;
			--error_count;
			break;
		case Letter::Unknown:
			break;
		default:
			break;
		}
	}

	void error()
	{
		last_error.restart();
		entry_no_error = 0;
		++error_count;
		current_word_perfect = false;
	}

	void reset()
	{
		started = false;
		entry_count = 0;
		entry_no_error = 0;
		error_count = 0;
		current_word_perfect = true;
		correct_word_count = 0;
		perfect_word_count = 0;

		clock.restart();
		last_error.restart();
	}

	bool nextWord(uint32_t skipped)
	{
		if (skipped)
		{
			error();
			error_count += skipped;
		}
		else if (current_word_perfect)
		{
			++perfect_word_count;
		}
		bool is_perfect = current_word_perfect;
		current_word_perfect = true;
		return is_perfect;
	}

	int32_t getElapsedSeconds() const
	{
		return clock.getElapsedTime().asSeconds();
	}

	int32_t getElapsedMilliseconds() const
	{
		return clock.getElapsedTime().asMilliseconds	();
	}

	void pop()
	{
		error();
	}

	float getAccuracy() const
	{
		if (getElapsedSeconds() < 1)
			return 0.0f;

		const float entries(entry_count);
		const float errors(error_count);
		const float accuracy((entries - errors) / entries);

		return std::max(0.0f, accuracy);
	}

	float getWPM()const
	{
		if (getElapsedSeconds() < 1)
			return 0.0f;

		const float entries(entry_count);
		const float errors(error_count);
		const float time(getElapsedMilliseconds() * 0.001f / 60.0f);
		const float wpm((entries * 0.2f - errors) / time);

		return std::max(0.0f, wpm);
	}

	bool started;

	uint32_t entry_no_error;
	uint32_t entry_count;
	uint32_t error_count;
	uint32_t correct_word_count;
	uint32_t perfect_word_count;
	bool current_word_perfect;

	sf::Clock clock;
	sf::Clock last_error;
};
