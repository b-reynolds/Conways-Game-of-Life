#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf {
	class RenderWindow;
}

class Cell
{

 public:

	Cell();
	explicit Cell(const sf::Vector2i& position, const unsigned int& size);

	void draw(sf::RenderWindow& window) const;

	sf::Vector2i position() const;

	void set_alive_colour(const sf::Color& colour);

	void set_alive(const bool &alive);
	bool alive() const;

 private:

	static const sf::Color kClrDead;
	static const sf::Color kClrAlive;
	static const sf::Color kClrOutline;

	sf::Color clr_alive_;
		
	sf::RectangleShape shape_;
	sf::Vector2i position_;

	bool alive_;

};

