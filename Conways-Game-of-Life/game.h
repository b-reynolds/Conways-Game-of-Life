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

	const unsigned int kCellSizeDefault = 16;
	const unsigned int kIterationsPerSecondMaxDefault = 25;

	const int kCellColourRDefault = 0;
	const int kCellColourGDefault = 196;
	const int kCellColourBDefault = 0;

	const sf::Keyboard::Key kKeyToggleActive = sf::Keyboard::Space;
	const sf::Keyboard::Key kKeyClear = sf::Keyboard::C;
	const sf::Mouse::Button kButtonAlive = sf::Mouse::Left;
	const sf::Mouse::Button kButtonDead = sf::Mouse::Right;

	sf::RenderWindow* window_;
	Timer timer_;

	std::vector<std::vector<Cell>> cells_;
	std::vector<std::pair<Cell*, bool>> changes_;

	unsigned int rows_;
	unsigned int columns_;

	unsigned int generations_;
	unsigned int population_;

	unsigned int cell_size;
	unsigned int iterations_per_second_max;

	bool toggle_was_pressed_;
	bool active_;

	void handle_input();

	void step();

	void reset();

	bool cell_within_bounds(const sf::Vector2i& position) const;
	std::vector<Cell*> get_neighbours(const Cell& node);

};

