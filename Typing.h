#pragma once
#include <string>
#include <vector>
#include "Letter.h"
#include <fstream>
#include <string>
#include <cctype>
class Typing
{
public:
	Typing(const char* textSource);
	void ChangeState(char letter);
	std::vector<Letter> GetLetters();
private:
	std::vector<Letter> letters;
	int currentInd;
	int wordsAmount;
	void ResourceLoading(const char* textSource);
};

