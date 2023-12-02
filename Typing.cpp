#include "Typing.h"


Typing::Typing()
{
	ResourceLoading();
}

void Typing::ChangeState(char letter)
{
	if (letter == '\b')
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

int Typing::GetCurrentInd()
{
	return currentInd;
}

std::vector<Letter> Typing::GetLetters()
{
	return letters;
}

void Typing::ResourceLoading()
{
	const char* textSource = "C:\\Users\\vanas\\OneDrive\\Рабочий стол\\3 курс\\СП\\Typing speed\\resources\\words.txt";
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



