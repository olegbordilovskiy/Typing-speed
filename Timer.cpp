#include "Timer.h"

Timer::Timer()
{
	timerRunning = false;
	availableTime = 5;
}

void Timer::StartTimer(int seconds)
{
	timerRunning = true;
	availableTime = seconds;
	startTime = std::chrono::steady_clock::now();
}

double Timer::GetAvailableTime()
{
	if (timerRunning) {
		auto elapsedTime = std::chrono::steady_clock::now() - startTime;
		double remainingTime = availableTime - std::chrono::duration_cast<std::chrono::duration<double>>(elapsedTime).count();
		if (remainingTime < 0) remainingTime = 0;
		return remainingTime;
	}
	else
		return 0;
}
