#include "Timer.h"

Timer::Timer()
{
	timerRunning = false;
	totalTime = 0;
}

void Timer::StartTimer(int seconds)
{
	timerRunning = true;
	totalTime = seconds;
	startTime = std::chrono::steady_clock::now();
}

float Timer::GetAvailableTime()
{
	if (timerRunning) {
		auto elapsedTime = std::chrono::steady_clock::now() - startTime;
		float remainingTime = totalTime - std::chrono::duration_cast<std::chrono::duration<double>>(elapsedTime).count();
		if (remainingTime < 0) remainingTime = 0;
		return remainingTime;
	}
	else
		return 0;
}

int Timer::GetTotalTime()
{
	return this->totalTime;
}
