#include "Typing.h"
#include <fstream>
#include <string>

Typing::Typing(const char* textSource)
{
	ResourceLoading(textSource);
}

void Typing::ChangeState(char letter)
{

}

void Typing::ResourceLoading(const char* textSource)
{
	std::ifstream wordsFile(textSource);
	if (!(wordsFile.is_open()))
	{
		return;
	}
	char letterSymbol;
	while (wordsFile.get(letterSymbol)) {
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



