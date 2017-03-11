#pragma once
#include <SFML/System/Clock.hpp>

class Timer
{

public:

	Timer();
	explicit Timer(const float& interval);
	bool elapsed();

private:

	sf::Clock clock_;
	float interval_;
	float last_tick_;

};

