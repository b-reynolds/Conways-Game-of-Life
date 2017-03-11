#include "timer.h"

Timer::Timer()
{
	interval_ = 0.0f;
	last_tick_ = clock_.getElapsedTime().asSeconds();
}

Timer::Timer(const float& interval)
{
	interval_ = interval;
	last_tick_ = clock_.getElapsedTime().asSeconds();
}

bool Timer::elapsed()
{
	if(clock_.getElapsedTime().asSeconds() - last_tick_ > interval_)
	{
		last_tick_ = clock_.getElapsedTime().asSeconds();
		return true;
	}
	return false;
}
