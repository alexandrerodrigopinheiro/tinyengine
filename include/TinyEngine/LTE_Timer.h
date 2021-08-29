#ifndef TINYENGINE_LTE_TIMER_H_
#define TINYENGINE_LTE_TIMER_H_

#pragma once

#include <ctime>
#include <chrono>
#include <inttypes.h>

const int64_t LTE_RESOLUTION = 1000000000L; // = 1 second.
const float LTE_INVERSE_RESOLUTION = 1.0f / LTE_RESOLUTION;

class LTE_Timer {
public:
	virtual ~LTE_Timer() {}
	// Returns the current time in ticks, which is an arbitrary
	// measure of time defined by the timer implementation.
	virtual int64_t Now() const = 0;
	// Returns the current time minus the previous time, in ticks.
	virtual int64_t Time() const = 0;
	// Returns the current time in seconds. The timer MUST start at 0.0.
	virtual float TimeInSeconds() const = 0;
	// Returns the resolution of the timer. Or in other words,
	// the number of ticks per second.
	virtual int64_t Resolution() const = 0;
	// Returns the inverse resolution of the timer. Or in other words,
	// the number of seconds per tick.
	virtual float InverseResolution() const = 0;
	// Returns the current number of frames per second.
	virtual float FrameRate() const = 0;
	// Returns the time in seconds between the last update and the current one.
	virtual float TimePerFrame() const = 0;
	// Recalculates the frame rate based on the previous
    // call to update. We assume that update is called in every frame.
	virtual void Update() = 0;
	// Reset the timer to 0.
	virtual void Reset() = 0;
};

class LTE_NanoTimer : public LTE_Timer {
public:
	typedef std::chrono::steady_clock clock;
	typedef std::chrono::nanoseconds resolution_type;
	//typedef int64_t interval;
	//typedef std::chrono::time_point<clock> time_point;
	LTE_NanoTimer() { Reset(); }
	virtual int64_t Now() const {
		auto now = clock::now();
		auto duration = now.time_since_epoch();
		return std::chrono::duration_cast<resolution_type>(duration).count();
	}
	virtual int64_t Time() const { return Now() - _startTime; }
	virtual float TimeInSeconds() const { return Time() * LTE_INVERSE_RESOLUTION; }
	virtual int64_t Resolution() const { return LTE_RESOLUTION; }
	virtual float InverseResolution() const { return LTE_INVERSE_RESOLUTION; }
	virtual float FrameRate() const { return _fps; }
	virtual float TimePerFrame() const { return _tpf; }
	virtual void Update() {
		_tpf = (Time() - _previousTime) * LTE_INVERSE_RESOLUTION;
		_fps = 1.0f / _tpf;
		_previousTime = Time();
	}
	virtual void Reset() {
		_startTime = Now();
		_previousTime = 0;
	}
protected:
	int64_t _startTime {0}, _previousTime {0};
	float _tpf {0}, _fps {0};
// private:
// 	static const int64_t RESOLUTION;
//	static const float INVERSE_RESOLUTION;
};

class LTE_FrameRateTimer : public LTE_NanoTimer {
public:
	LTE_FrameRateTimer() { Reset(); }
	virtual float TimePerFrame() const { return (1000.0f / _fps) / 1000.0f; }
	virtual void Update() {
		_tpf = (Time() - _previousTime) * InverseResolution();
		//_fps = 1.0f / _tpf;
		_previousTime = Time();
		_second_counter += _tpf;
		_frame_count++;
		if (_second_counter >= 1.0f) {
			_second_counter = 0.0f;
			_fps = _frame_count;
			_frame_count = 0;
		}
	}
protected:
	float _second_counter {0};
	uint32_t _frame_count {0};
};

#endif
