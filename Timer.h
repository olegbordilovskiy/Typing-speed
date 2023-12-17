#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	void StartTimer(int seconds);
	float GetAvailableTime();
	int GetTotalTime();
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	int totalTime;
	bool timerRunning;
};

