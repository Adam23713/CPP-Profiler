#include "../Profilers.h"
#include <chrono>
#include <thread>

void CountingFunction(int end)
{
	FUNCTION_TIMER

	for (int i = 0; i < end; i++) ;
}

void SleepFunction(int time)
{
	FUNCTION_TIMER

	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

int main(int argc, char *argv[])
{
	{
		BLOCK_TIMER("Test block")
		CountingFunction(200000000);
		CountingFunction(100000000);
		CountingFunction(100000000);
		SleepFunction(100);
		std::cout << "Please press enter..." << std::endl;
		std::cin.get();
	}
	TimerMeasurerCollector::Instance().WriteResults();
	std::cin.get();
	return 0;
}

