#pragma once
#include <string>
#include <vector>
#include "Letter.h"
class Typing
{
public:
	Typing(const char* wordsSource);
	void ChangeState(wchar_t symbol);
private:
	std::vector<Letter> letters;
	int currentInd;
	int wordsAmount;
};

