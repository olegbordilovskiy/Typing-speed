#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	void StartTimer(int seconds);
	double GetAvailableTime();
	bool IsTimerRunning();
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	int availableTime;
	bool timerRunning;
};

