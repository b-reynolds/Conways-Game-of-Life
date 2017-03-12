#include "cell.h"
#include <SFML/Graphics/RenderWindow.hpp>

sf::Color const Cell::kClrDead = sf::Color(25, 25, 25);
sf::Color const Cell::kClrOutline = sf::Color(35, 35, 35);

Cell::Cell() : Cell(sf::Vector2i(0, 0), 32) {}

Cell::Cell(const sf::Vector2i& position, const unsigned int& size)
{
	set_alive(false);
	shape_.setOutlineThickness(1);
	shape_.setOutlineColor(kClrOutline);
	shape_.setSize(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
	position_ = position;
	shape_.setPosition(sf::Vector2f(position.x * shape_.getSize().x, position.y * shape_.getSize().y));
}

void Cell::draw(sf::RenderWindow& window) const
{
	window.draw(shape_);
}

sf::Vector2i Cell::position() const
{
	return position_;
}

void Cell::set_alive_colour(const sf::Color& colour)
{
	clr_alive_ = colour;
}

void Cell::set_alive(const bool& alive)
{
	alive_ = alive;
	shape_.setFillColor(alive_ ? clr_alive_ : kClrDead);
}

bool Cell::alive() const
{
	return alive_;
}