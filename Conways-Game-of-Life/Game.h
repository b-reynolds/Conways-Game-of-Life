#pragma once

namespace sf {
	class RenderWindow;
}

class Game
{

 public:

	void update(sf::RenderWindow& window, const float& delta_time);
	void draw(sf::RenderWindow& window);

 private:

};

