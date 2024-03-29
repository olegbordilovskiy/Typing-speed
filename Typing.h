#pragma once
#include <string>
#include <vector>
#include "Letter.h"
#include "Timer.h"
#include "Timer.h"
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <random>

class Typing
{
public:
	Typing();
	void ChangeState(char symbol);
	int GetCurrentInd();
	void SetTimeForTesting(int seconds);
	int GetTimeForTesting();
	void StartTyping();
	float CheckTime();
	int GetWPM();
	int GetAccuracy();
	int GetTotalTime();
	std::vector<Letter> GetLetters();
private:
	int timeForTesting;
	int currentInd;
	int wordsAmount;
	void ResourceLoading();
	std::vector<Letter> letters;
	Timer timer;

};

