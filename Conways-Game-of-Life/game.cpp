#include "game.h"
#include "global.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <config_parser.h>

Game::Game(sf::RenderWindow* window)
{
	ConfigParser config_parser;
	auto result = config_parser.parse(kConfigFile);


	std::string output;

	if (!ConfigParser::find(result, "iterations_per_second_max", output) || !ConfigParser::to_uint(output, iterations_per_second_max))
	{
		iterations_per_second_max = kIterationsPerSecondMaxDefault;
	}
	if (!ConfigParser::find(result, "cell_size", output) || !ConfigParser::to_uint(output, cell_size) || cell_size == 0)
	{
		cell_size = kCellSizeDefault;
	}

	int cell_colour_r;
	int cell_colour_g;
	int cell_colour_b;

	if(!ConfigParser::find(result, "cell_colour_r", output) || !ConfigParser::to_int(output, cell_colour_r) || cell_colour_r < 0 || cell_colour_r > 255 ||
		!ConfigParser::find(result, "cell_colour_g", output) || !ConfigParser::to_int(output, cell_colour_g) || cell_colour_g < 0 || cell_colour_g > 255 ||
		!ConfigParser::find(result, "cell_colour_b", output) || !ConfigParser::to_int(output, cell_colour_b) || cell_colour_b < 0 || cell_colour_b > 255)
	{
		cell_colour_r = kCellColourRDefault;
		cell_colour_g = kCellColourGDefault;
		cell_colour_b = kCellColourBDefault;
	}

	window_ = window;
	generations_ = 0;
	population_ = 0;
	toggle_was_pressed_ = false;
	timer_ = Timer(1000.0f / iterations_per_second_max / 1000.0f);
	active_ = false;

	sf::Vector2u window_size = window_->getSize();

	rows_ = window_size.x / cell_size;
	columns_ = window_size.y / cell_size;

	cells_.resize(rows_, std::vector<Cell>(columns_, Cell()));

	for(unsigned int x = 0; x < rows_; ++x)
	{
		for(unsigned int y = 0; y < columns_; ++y)
		{
			cells_[x][y] = Cell(sf::Vector2i(x, y), cell_size);
			cells_[x][y].set_alive_colour(sf::Color(cell_colour_r, cell_colour_g, cell_colour_b));
		}
	}
}

void Game::update()
{
	window_->setTitle(kWinTitle + std::string("(Generation: " + std::to_string(generations_) + ", Population: " + std::to_string(population_) + ")"));

	handle_input();

	if(active_ && timer_.elapsed())
	{
		step();
	}
}

void Game::draw()
{
	for (auto & vector : cells_)
	{
		for (auto & node : vector)
		{
			node.draw(*window_);
		}
	}
}

void Game::handle_input()
{
	if (sf::Mouse::isButtonPressed(kButtonAlive))
	{
		sf::Vector2i node_position = sf::Mouse::getPosition(*window_) / static_cast<int>(cell_size);
		if (cell_within_bounds(node_position))
		{
			cells_[node_position.x][node_position.y].set_alive(true);
		}
	}
	else if(sf::Mouse::isButtonPressed(kButtonDead))
	{
		sf::Vector2i node_position = sf::Mouse::getPosition(*window_) / static_cast<int>(cell_size);
		if (cell_within_bounds(node_position))
		{
			cells_[node_position.x][node_position.y].set_alive(false);
		}
	}

	if(sf::Keyboard::isKeyPressed(kKeyClear))
	{
		reset();
	}
	else if(sf::Keyboard::isKeyPressed(kKeyToggleActive))
	{
		if (!toggle_was_pressed_)
		{
			active_ = !active_;
		}
		toggle_was_pressed_ = true;
	}
	else
	{
		toggle_was_pressed_ = false;
	}
}

void Game::step()
{
	population_ = 0;

	for(auto & vector : cells_)
	{
		for(auto & cell : vector)
		{
			unsigned int alive_neighbours = 0;
			unsigned int dead_neighbours = 0;

			for (auto & neighbour : get_neighbours(cell))
			{
				neighbour->alive() ? alive_neighbours++ : dead_neighbours++;
			}

			if(cell.alive())
			{
				population_++;
				if (alive_neighbours < 2 || alive_neighbours > 3)
				{
					changes_.push_back(std::pair<Cell*, bool>(&cell, false));
				}
			}
			else if(alive_neighbours == 3)
			{
				changes_.push_back(std::pair<Cell*, bool>(&cell, true));
			}		
		}
	}

	if(changes_.empty())
	{
		active_ = false;
		return;
	}

	while (!changes_.empty())
	{
		std::pair<Cell*, bool> change = changes_.back();
		change.first->set_alive(change.second);
		changes_.pop_back();
	}

	generations_++;
}

void Game::reset()
{
	for(auto & vector : cells_)
	{
		for(auto & node : vector)
		{
			node.set_alive(false);
		}
	}

	changes_.clear();
	generations_ = 0;
	population_ = 0;
	active_ = false;
}

bool Game::cell_within_bounds(const sf::Vector2i& position) const
{
	return position.x >= 0 && position.y >= 0 && position.x < static_cast<int>(rows_) && position.y < static_cast<int>(columns_);
}

std::vector<Cell*> Game::get_neighbours(const Cell& node)
{
	std::vector<Cell*> neighbours;

	sf::Vector2i node_position = node.position();

	sf::Vector2i north = sf::Vector2i(node_position.x, node_position.y - 1);
	if(cell_within_bounds(north))
	{
		neighbours.push_back(&cells_[north.x][north.y]);
	}

	sf::Vector2i north_east = sf::Vector2i(node_position.x + 1, node_position.y - 1);
	if(cell_within_bounds(north_east))
	{
		neighbours.push_back(&cells_[north_east.x][north_east.y]);
	}

	sf::Vector2i east = sf::Vector2i(node_position.x + 1, node_position.y);
	if (cell_within_bounds(east))
	{
		neighbours.push_back(&cells_[east.x][east.y]);
	}

	sf::Vector2i south_east = sf::Vector2i(node_position.x + 1, node_position.y + 1);
	if (cell_within_bounds(south_east))
	{
		neighbours.push_back(&cells_[south_east.x][south_east.y]);
	}

	sf::Vector2i south = sf::Vector2i(node_position.x, node_position.y + 1);
	if (cell_within_bounds(south))
	{
		neighbours.push_back(&cells_[south.x][south.y]);
	}

	sf::Vector2i south_west = sf::Vector2i(node_position.x - 1, node_position.y + 1);
	if (cell_within_bounds(south_west))
	{
		neighbours.push_back(&cells_[south_west.x][south_west.y]);
	}

	sf::Vector2i west = sf::Vector2i(node_position.x - 1, node_position.y);
	if (cell_within_bounds(west))
	{
		neighbours.push_back(&cells_[west.x][west.y]);
	}

	sf::Vector2i north_west = sf::Vector2i(node_position.x - 1, node_position.y - 1);
	if (cell_within_bounds(north_west))
	{
		neighbours.push_back(&cells_[north_west.x][north_west.y]);
	}

	return neighbours;
}
