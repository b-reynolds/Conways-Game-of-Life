#pragma once
#include <vector>
#include "cell.h"
#include "timer.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace sf {
	class RenderWindow;
}

class Game
{

 public:

	explicit Game(sf::RenderWindow* window);

	void update();
	void draw();

 private:

	const unsigned int kCellSize = 16;
	const int kIterationsPerSecondMax = 10;

	const sf::Keyboard::Key kKeyToggleActive = sf::Keyboard::Space;
	const sf::Keyboard::Key kKeyClear = sf::Keyboard::C;
	const sf::Mouse::Button kButtonAlive = sf::Mouse::Left;
	const sf::Mouse::Button kButtonDead = sf::Mouse::Right;

	const char* kWinTitle = "Conway's Game of Life ";

	sf::RenderWindow* window_;
	Timer timer_;

	std::vector<std::vector<Cell>> cells_;
	std::vector<std::pair<Cell*, bool>> changes_;

	unsigned int rows_;
	unsigned int columns_;

	unsigned int generations_;
	unsigned int population_;

	bool toggle_was_pressed_;
	bool active_;

	void handle_input();

	void step();

	void reset();

	bool cell_within_bounds(const sf::Vector2i& position) const;
	std::vector<Cell*>& get_neighbours(const Cell& node);

};

