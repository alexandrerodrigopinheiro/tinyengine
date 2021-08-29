#ifndef TINYENGINE_LTE_FRAMER_H_
#define TINYENGINE_LTE_FRAMER_H_

#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include "LTE_Utils.h"
#include "LTE_Clock.h"

class LTE_Framer
{
public:
	LTE_Framer() = default;
	~LTE_Framer() = default;
	LTE_Framer(LTE_Framer const&) = default;
	LTE_Framer(std::size_t maximum)
		: _timer(LTE_Clock())
		, _frame(0)
		, _ticks(1)
		, _maximum(maximum)
		, _play(false)
	{
	}
	void Play()
	{
		this->_play = true;
		this->_timer.Start();
	}
	void Pause()
	{
		this->_play = false;
		this->_timer.Pause();
	}
	void Reset()
	{
		this->_frame = 0;
		this->Pause();
	}
	void Replay()
	{
		this->_frame = 0;
		this->Play();
	}
	void Run(bool pingpong = false, float rate = (1.f / 30.f))
	{
		if (this->_play && pingpong == true && this->_timer.GetSeconds() >= rate)
		{
			this->_frame = LTE_Wrap(std::size_t(this->_frame + this->_ticks), static_cast<std::size_t>(0), this->_maximum);
			this->_timer.Start();
		}
		else if (this->_play && pingpong == false && this->_timer.GetSeconds() >= rate)
		{
			this->_frame = LTE_Clamp(std::size_t(this->_frame + this->_ticks), static_cast<std::size_t>(0), this->_maximum);
			this->_timer.Start();
		}
	}
	void SetTicks(std::size_t ticks)
	{
		this->_ticks = ticks;
	}
	void SetMaximun(std::size_t maximum)
	{
		this->_maximum = maximum;
	}
	std::size_t GetTicks() const
	{
		return this->_ticks;
	}
	std::size_t GetMaximun() const
	{
		return this->_maximum;
	}
	bool IsPlay() const
	{
		return this->_play;
	}
	std::size_t GetFrame() const
	{
		return this->_frame;
	}
	bool Bound() const
	{
		return this->_frame == this->_maximum;
	}
protected:
	LTE_Clock _timer { LTE_Clock() };
	std::size_t _frame { 0 };
	std::size_t _ticks { 1 };
	std::size_t _maximum { 0 };
	bool _play { false };
};

#endif /* TINYENGINE_LTE_FRAMER_H_ */
