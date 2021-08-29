#ifndef TINYENGINE_LTE_CLOCK_H_
#define TINYENGINE_LTE_CLOCK_H_

#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <SDL/SDL_timer.h>


class LTE_Clock
{
public:
	LTE_Clock()
		: _startTicks(0)
		, _pausedTicks(0)
		, _paused(true)
		, _started(false)
	{
	}
	void Start()
	{
		this->_started = true;
		this->_paused = false;
		this->_startTicks = SDL_GetTicks();
		this->_pausedTicks = 0;
	}
	void Stop()
	{
		this->_started = false;
		this->_paused = false;
		this->_startTicks = 0;
		this->_pausedTicks = 0;
	}
	void Pause()
	{
		if (this->_started && !this->_paused)
		{
			this->_paused = true;
			this->_pausedTicks = SDL_GetTicks() - this->_startTicks;
			this->_startTicks = 0;
		}
	}
	void Unpause()
	{
		if (this->_started && this->_paused)
		{
			this->_paused = false;
			this->_startTicks = SDL_GetTicks() - this->_pausedTicks;
			this->_pausedTicks = 0;
		}
	}
	uint32_t GetMSeconds() const
	{
		uint32_t time = 0;
		if (this->_started)
		{
			if (this->_paused)
			{
				time = this->_pausedTicks;
			}
			else
			{
				time = SDL_GetTicks() - this->_startTicks;
			}
		}
		return time;
	}
	float GetSeconds() const
	{
		return static_cast<float>(this->GetMSeconds()) / 1000.f;
	}
	bool IsStarted() const
	{
		return this->_started;
	}
	bool IsPaused() const
	{
		return this->_paused && this->_started;
	}
private:
	uint32_t _startTicks { 0 };
	uint32_t _pausedTicks { 0 };
	bool _paused  { false };
	bool _started { false };
};

#endif /* TINYENGINE_LTE_CLOCK_H_ */
