#pragma once
#include <string>
#include <vector>
#include "Letter.h"
class Typing
{
public:
	Typing(const char* textSource);
	void ChangeState(char letter);
private:
	std::vector<Letter> letters;
	int currentInd;
	int wordsAmount;
	void ResourceLoading(const char* textSource);
};

