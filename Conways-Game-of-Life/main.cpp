#include <SFML/Graphics.hpp>
#include <config_parser.h>
#include "game.h"
#include "global.h"

int main()
{
	const unsigned int kWinWidthDefault = 800;
	const unsigned int kWinHeightDefault = 600;

	ConfigParser config_parser;

	auto result = config_parser.parse(kConfigFile);

	std::string output;

	unsigned int win_width = NULL;
	if(!ConfigParser::find(result, "win_width", output) || !ConfigParser::to_uint(output, win_width) || win_width == NULL)
	{
		win_width = kWinWidthDefault;
	}

	unsigned int win_height = NULL;
	if (!ConfigParser::find(result, "win_height", output) || !ConfigParser::to_uint(output, win_height) || win_height == NULL)
	{
		win_height = kWinHeightDefault;
	}

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), kWinTitle, sf::Style::Close);

	Game game(&window);

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		game.update();

		window.clear();

		game.draw();

		window.display();
			
	}

	return 0;
}
