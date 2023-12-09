#include "Timer.h"

Timer::Timer()
{
	timerRunning = false;
	availableTime = 0.0;
}

void Timer::StartTimer(int seconds)
{
	timerRunning = true;
	availableTime = seconds * 1000.0;
	startTime = std::chrono::steady_clock::now();
}

double Timer::GetAvailableTime()
{
	auto availableTime = std::chrono::steady_clock::now() - startTime;
	return std::chrono::duration_cast<std::chrono::duration<double>>(availableTime).count();
}
