#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL_timer.h"

class Timer
{
public:

	Timer()
	{
		Start();
	}

	void Start()
	{
		running = true;
		startTime = SDL_GetTicks();
	}

	void Stop()
	{
		running = false;
		stopTime = SDL_GetTicks();
	}

	uint32 Read() const
	{
		if (running)
			return SDL_GetTicks() - startTime;
		else
			return stopTime - startTime;
	}

	float ReadSec() const
	{
		if (running)
			return float(SDL_GetTicks() - startTime) / 1000.0f;
		else
			return float(stopTime - startTime) / 1000.0f;
	}

private:

	bool running;
	uint32 startTime;
	uint32 stopTime;
};

#endif //__TIMER_H__