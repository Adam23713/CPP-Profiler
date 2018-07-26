#ifndef PROFILER_H
#define PROFILER_H

#include <map>
#include <mutex>
#include <string>
#include <limits>
#include <numeric>
#include <exception>
#include <iostream>
#include "Timer.h"
#include "ConsoleTable.h"

typedef std::chrono::nanoseconds TimeType;

#define FUNCTION_TIMER TimeMeasurer functionTimeMeasurer(__FUNCTION__);
#define BLOCK_TIMER(blockName) TimeMeasurer blockTimeMeasurer( std::string(blockName) + std::string(" in ") + std::string(__FUNCTION__) );


template<typename TimeType> class PerformanceResult
{
public:
	PerformanceResult() {}
	~PerformanceResult() = default;
	int GetCounter() const { return _timeResults.size(); }

	void SetTimeResult(int index, TimeType& time)
	{
		if (index >= _timeResults.size())
		{
			std::string what = "Time results array size: " + std::to_string(_timeResults.size()) + "\n Your Index: " + std::to_string(index);
			std::out_of_range ex(what);
		}
		_timeResults.at(index) = time; 
	}

	TimeType At(int index) const
	{ 
		if (index >= _timeResults.size())
		{
			std::string what = "Time results array size: " + std::to_string(_timeResults.size()) + "\n Your Index: " + std::to_string(index);
			std::out_of_range ex(what);
		}
		return _timeResults.at(index); 
	}

	void AddTimeResult(const TimeType& duration) 
	{
		_timeResults.push_back(duration);
	}

	TimeType GetSummary() const
	{
		return std::accumulate(_timeResults.begin(), _timeResults.end(), TimeType::zero() );
	}

	TimeType GetMinimum() const
	{
		return *(std::min_element(_timeResults.begin(), _timeResults.end()));
	}

	TimeType GetMaximum() const
	{
		return *(std::max_element(_timeResults.begin(), _timeResults.end()));
	}

	std::pair<TimeType, TimeType> GetMinMax() const
	{
		return std::minmax(_timeResults.begin(), _timeResults.end());
	}

protected:
	std::vector<TimeType> _timeResults;
};

class TimerMeasurerCollector
{

private:
	TimerMeasurerCollector() {};
	std::string ChronoTimeToStdString(TimeType time) const;

public:
	~TimerMeasurerCollector() = default;
	static TimerMeasurerCollector& Instance();
	void AddNewElement(const std::string& name, TimeType duration);
	void WriteResults(std::ostream *stream = &std::cout, bool desc = false) const;

private:
	mutable std::mutex _guardMutex;
	std::map<std::string, PerformanceResult<TimeType>> _results;
};

class TimeMeasurer
{
public:
	typedef std::chrono::nanoseconds TimeType;

public:
	TimeMeasurer(const std::string& name);
	~TimeMeasurer();

private:
	std::string _subjectName;
	Timer _curretTimer;
};

#endif
