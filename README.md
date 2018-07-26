# Code Profiling
This is a simple Profiler tool for C++. You can use two macros to measure your program code speed.

- FUNCTION_TIMER
- BLOCK_TIMER

For their use, see the example below.

# Prerequisites
- You need to my Console-Table project. You can download it from GitHub: https://github.com/Adam23713/Console-Table
- A C++ compiler, for example  g++, VS C++ etc.

# Compiling
g++ your_program.cpp Profilers.cpp ConsoleTable.cpp -o program

# Example
```C++
#include "Profilers.h"
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
```
# Output
![alt text](https://user-images.githubusercontent.com/12465316/43267409-29be87e0-90ee-11e8-92af-19352980cd95.png)


# Authors
Kertész Ádám
<br>My webpage: https://callprog.hu
 
# License
This project is licensed under the Simplified BSD License or FreeBSD License - see the LICENSE.md file for details
