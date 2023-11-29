#include "Typing.h"
#include <fstream>
#include <string>

Typing::Typing(const char* wordsSource)
{
	std::ifstream wordsFile(wordsSource);
	if (!(wordsFile.is_open()))
	{
		return;
	}
}

void Typing::ChangeState(wchar_t symbol)
{

}



