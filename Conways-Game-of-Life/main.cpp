#include <SFML/Graphics.hpp>
#include "game.h"

int main()
{
	const unsigned int kWinWidth = 600;
	const unsigned int kWinHeight = 600;

	sf::RenderWindow window(sf::VideoMode(kWinWidth, kWinHeight), "", sf::Style::Close);

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
