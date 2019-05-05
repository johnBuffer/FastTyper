#include <SFML/Graphics.hpp>
#include <iostream>
#include <event_manager.hpp>
#include <transition.hpp>
#include "utils.hpp"
#include "challenge_words.hpp"
#include "theme.hpp"

int main(int argc, char** argv)
{
	std::cout << argc << std::endl;
	bool replay_mode(false);
	std::string replay_file("");
	if (argc > 1)
	{
		replay_mode = true;
		replay_file = std::string(argv[1]);
	}

	// Initialize window
	const uint32_t win_width(1600);
	const uint32_t win_height(900);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "FastTyper", sf::Style::Default, settings);
	sfev::EventManager event_manager(window);
	window.setVerticalSyncEnabled(true);

	sf::RenderTexture main_renderer;
	main_renderer.create(win_width, win_height);

	srand(time(NULL));
	ChallengeWords::init("");
	ChallengeWords challenge(win_width, win_height);

	// Event intialization
	event_manager.addEventCallback(sf::Event::Closed, [&](sfev::CstEv) {window.close(); });

	Replay replay;
	uint32_t action_count;
	uint32_t i(0);
	ReplayAction nextAction; (replay.getAction(i));
	if (!replay_mode)
	{
		std::cout << "Challenge mode" << std::endl;
		event_manager.addEventCallback(sf::Event::TextEntered, [&](sfev::CstEv ev) {challenge.addChar(ev.text.unicode); });
		event_manager.addKeyPressedCallback(sf::Keyboard::BackSpace, [&](sfev::CstEv ev) {challenge.removeChar(); });
		event_manager.addKeyPressedCallback(sf::Keyboard::Space, [&](sfev::CstEv ev) {challenge.nextWord(); });
		event_manager.addKeyPressedCallback(sf::Keyboard::Tab, [&](sfev::CstEv ev) {challenge.exportReplay(); });
	}
	else
	{
		std::cout << "Replay mode, file " << replay_file << std::endl;
		replay.loadFromFile(replay_file);
		action_count = replay.actionCount();

		if (i < action_count)
		{
			nextAction = replay.getAction(i);
			challenge.use(replay);
		}
	}

	sf::Clock clock;

	while (window.isOpen())
	{
		event_manager.processEvents();

		if (replay_mode)
		{
			if (clock.getElapsedTime().asMilliseconds() >= nextAction.timestamp && i < action_count)
			{
				if (nextAction.action == ReplayAction::ActionType::AddChar)
				{
					challenge.addChar(nextAction.char_code);
				}
				else if (nextAction.action == ReplayAction::ActionType::NextWord)
				{
					challenge.nextWord();
				}
				else if (nextAction.action == ReplayAction::ActionType::RemoveChar)
				{
					challenge.removeChar();
				}

				++i;
				if (i < action_count)
				{
					nextAction = replay.getAction(i);
				}
			}
		}

		challenge.update();

		// Draw
		window.clear(sf::Color(32, 32, 32));

		challenge.render(window);
		challenge.renderBloom(window);

		window.display();
	}

	return 0;
}