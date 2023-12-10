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
	void ChangeState(char letter);
	int GetCurrentInd();
	std::vector<Letter> GetLetters();
	void StartTyping(int secondsForTimer);
	double CheckTime();
private:
	std::vector<Letter> letters;
	Timer timer;
	int currentInd;
	int wordsAmount;
	void ResourceLoading();
	

};

