#pragma once
#include <string>
#include <vector>
#include "Letter.h"
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
private:
	std::vector<Letter> letters;
	int currentInd;
	int wordsAmount;
	void ResourceLoading();

};

