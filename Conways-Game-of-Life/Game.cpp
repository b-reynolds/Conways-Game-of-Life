#include "game.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

Game::Game(sf::RenderWindow* window)
{
	toggle_was_pressed_ = false;
	generations_ = 0;
	population_ = 0;
	window_ = window;
	timer_ = Timer((1000.0f / kIterationsPerSecondMax) / 1000.0f);
	active_ = false;

	sf::Vector2u window_size = window->getSize();

	rows_ = window_size.x / kCellSize;
	columns_ = window_size.y / kCellSize;

	nodes_.resize(rows_, std::vector<Cell>(columns_));

	for(unsigned int x = 0; x < rows_; ++x)
	{
		for(unsigned int y = 0; y < columns_; ++y)
		{
			nodes_[x][y] = Cell(sf::Vector2i(x, y), kCellSize);
		}
	}
}

void Game::update()
{
	window_->setTitle(kWinTitle + std::string("(Generation: " + std::to_string(generations_) + ", Population: " + std::to_string(population_) + ")"));

	handle_input();

	if(!active_ || !timer_.elapsed())
	{
		return;
	}

	apply_changes();
	calculate_changes();

	active_ = !changes_.empty();
}

void Game::draw()
{
	for (auto & vector : nodes_)
	{
		for (auto & node : vector)
		{
			node.draw(*window_);
		}
	}
}

void Game::handle_input()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i node_position = sf::Mouse::getPosition(*window_) / static_cast<int>(kCellSize);
		if (node_within_bounds(node_position))
		{
			nodes_[node_position.x][node_position.y].set_alive(true);
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i node_position = sf::Mouse::getPosition(*window_) / static_cast<int>(kCellSize);
		if (node_within_bounds(node_position))
		{
			nodes_[node_position.x][node_position.y].set_alive(false);
		}
	}
	if(sf::Keyboard::isKeyPressed(kKeyClear))
	{
		reset();
	}
	else if (sf::Keyboard::isKeyPressed(kKeyToggleActive))
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

void Game::apply_changes()
{
	while (!changes_.empty())
	{
		std::pair<Cell*, bool> change = changes_.back();
		change.first->set_alive(change.second);
		changes_.pop_back();
	}
}

void Game::calculate_changes()
{
	population_ = 0;
	for (unsigned int x = 0; x < rows_; ++x)
	{
		for (unsigned int y = 0; y < columns_; ++y)
		{
			Cell* node = &nodes_[x][y];
			auto neighbours = get_neighbours(*node);

			unsigned int alive_neighbours = 0;
			unsigned int dead_neighbours = 0;

			for (auto & neighbour : neighbours)
			{
				neighbour->alive() ? alive_neighbours++ : dead_neighbours++;
			}

			if (node->alive())
			{
				if (alive_neighbours < 2 || alive_neighbours > 3)
				{
					changes_.push_back(std::pair<Cell*, bool>(node, false));
				}
				population_++;
			}
			else
			{
				if (alive_neighbours == 3)
				{
					changes_.push_back(std::pair<Cell*, bool>(node, true));
				}
			}
		}
	}
	generations_++;
}

void Game::reset()
{
	for(auto & vector : nodes_)
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

bool Game::node_within_bounds(const sf::Vector2i& position) const
{
	return position.x >= 0 && position.y >= 0 && position.x < static_cast<int>(rows_) && position.y < static_cast<int>(columns_);
}

std::vector<Cell*> Game::get_neighbours(const Cell& node)
{
	std::vector<Cell*> neighbours;

	sf::Vector2i node_position = node.position();

	sf::Vector2i north = sf::Vector2i(node_position.x, node_position.y - 1);
	if(node_within_bounds(north))
	{
		neighbours.push_back(&nodes_[north.x][north.y]);
	}

	sf::Vector2i north_east = sf::Vector2i(node_position.x + 1, node_position.y - 1);
	if(node_within_bounds(north_east))
	{
		neighbours.push_back(&nodes_[north_east.x][north_east.y]);
	}

	sf::Vector2i east = sf::Vector2i(node_position.x + 1, node_position.y);
	if (node_within_bounds(east))
	{
		neighbours.push_back(&nodes_[east.x][east.y]);
	}

	sf::Vector2i south_east = sf::Vector2i(node_position.x + 1, node_position.y + 1);
	if (node_within_bounds(south_east))
	{
		neighbours.push_back(&nodes_[south_east.x][south_east.y]);
	}

	sf::Vector2i south = sf::Vector2i(node_position.x, node_position.y + 1);
	if (node_within_bounds(south))
	{
		neighbours.push_back(&nodes_[south.x][south.y]);
	}

	sf::Vector2i south_west = sf::Vector2i(node_position.x - 1, node_position.y + 1);
	if (node_within_bounds(south_west))
	{
		neighbours.push_back(&nodes_[south_west.x][south_west.y]);
	}

	sf::Vector2i west = sf::Vector2i(node_position.x - 1, node_position.y);
	if (node_within_bounds(west))
	{
		neighbours.push_back(&nodes_[west.x][west.y]);
	}

	sf::Vector2i north_west = sf::Vector2i(node_position.x - 1, node_position.y - 1);
	if (node_within_bounds(north_west))
	{
		neighbours.push_back(&nodes_[north_west.x][north_west.y]);
	}

	return neighbours;
}
