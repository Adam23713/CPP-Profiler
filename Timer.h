#ifndef TIMER_H
#define TIMER_H


#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <chrono>

class Timer
{
public:
	typedef std::conditional< std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
		std::chrono::steady_clock >::type Clock;
public:
	Timer(bool start = false)
	{
		if (true == start) Start();
	}

	void Start()
	{
		if (_runIt)
		{
			std::cerr << "ERROR: Timer already run!" << std::endl;
			return;
		}
		_runIt = true;
		_startTimePoint = Clock::now();
	}

	void Stop()
	{
		_runIt = false;
		_endTimePoint = Clock::now();
	}

	void Reset()
	{
		_runIt = false;
		Start();
	}

	template<typename Type> Type ElapsedTime() const
	{
		auto duration = (true == _runIt) ? (Clock::now() - _startTimePoint) : _endTimePoint - _startTimePoint;
		return std::chrono::duration_cast<Type>(duration);
	}

private:
	bool _runIt = false;
	Clock::time_point _startTimePoint;
	Clock::time_point _endTimePoint;
};

#endif
