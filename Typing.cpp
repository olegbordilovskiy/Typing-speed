#include "Typing.h"


Typing::Typing(const char* textSource)
{
	ResourceLoading(textSource);
}

void Typing::ChangeState(char letter)
{
	if (letter == '0x08')
	{
		currentInd--;
		return;
	}
	if (letter != letters[currentInd].GetLetter())
	{
		letters[currentInd].SetIsCorrect(false);
		return;
	}
	currentInd++;
}

std::vector<Letter> Typing::GetLetters()
{
	return std::vector<Letter>();
}

void Typing::ResourceLoading(const char* textSource)
{
	std::ifstream textFile(textSource);
	if (!(textFile.is_open()))
	{
		return;
	}

	char letterSymbol;
	while (textFile.get(letterSymbol)) {
		if (letterSymbol != '\n') 
		{
			Letter letter(letterSymbol);
			letters.push_back(letter);
		}
		else 
		{
			Letter letter(' ');
			letters.push_back(letter);
		}
	}
}



