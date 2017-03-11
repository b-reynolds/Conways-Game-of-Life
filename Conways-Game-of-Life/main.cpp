#include <SFML/Graphics.hpp>

int main()
{
	const unsigned int kWinWidth = 800;
	const unsigned int kWinHeight = 600;
	const char* kWinTitle = "Conway's Game of Life";

	sf::RenderWindow window(sf::VideoMode(kWinWidth, kWinHeight), kWinTitle, sf::Style::Close);

	sf::Clock delta_clock;
	float delta_time = 0.0f;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}

			window.clear();

			window.display();

			delta_time = delta_clock.restart().asSeconds();
		}		
	}

	return 0;
}