#include "Profilers.h"

TimeMeasurer::TimeMeasurer(const std::string& name) : _subjectName(name), _curretTimer(true)
{
}

TimeMeasurer::~TimeMeasurer()
{
	auto duration = _curretTimer.ElapsedTime<TimeType>();
	TimerMeasurerCollector::Instance().AddNewElement(_subjectName, duration);
}

std::string TimerMeasurerCollector::ChronoTimeToStdString(TimeType time) const
{
	auto nanoTime = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();

	if (nanoTime >= 1000000000)
	{
		auto sec = nanoTime / (double)1000000000;

		if (sec >= 3600)
			return std::to_string(sec / (double)3600) + " hour";
		else if (sec >= 60)
			return std::to_string(sec / (double)60) + " min";

		return std::to_string(sec) + " sec";
	}
	else if (nanoTime >= 1000000)
		return std::to_string(nanoTime / (double)1000000) + " ms";
	else if (nanoTime >= 1000)
		return std::to_string(nanoTime / (double)1000) + " us";
	
	return std::to_string(nanoTime) + " ns";
}

TimerMeasurerCollector& TimerMeasurerCollector::Instance()
{
	static TimerMeasurerCollector _instance;
	return _instance;
}

void TimerMeasurerCollector::WriteResults(std::ostream *stream, bool desc) const
{
	std::lock_guard<std::mutex> lock(_guardMutex);  //Lock function

	ConsoleTable table(6);
	table.AddNewRow({ "Name", "Runs", "Minimum", "Maximum", "Total Time","Average Time"});

	for(const std::pair<std::string, PerformanceResult<TimeType>>& result : _results)
	{
		std::forward_list<std::string> row;
		TimeType max = result.second.GetMaximum();
		TimeType min = result.second.GetMinimum();

		row.push_front(ChronoTimeToStdString(result.second.GetSummary() / result.second.GetCounter()));
		row.push_front(ChronoTimeToStdString(result.second.GetSummary()));
		row.push_front(ChronoTimeToStdString(max));
		row.push_front(ChronoTimeToStdString(min));
		row.push_front(std::to_string(result.second.GetCounter()));
		row.push_front(result.first);

		table.AddNewRow(row);
	}
	table.WriteTable(Align::Center, stream);
}

void TimerMeasurerCollector::AddNewElement(const std::string& name, TimeType duration)
{
	std::lock_guard<std::mutex> lock(_guardMutex); //Lock Function
	_results[name].AddTimeResult(duration);
}
