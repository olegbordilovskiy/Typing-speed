#include "Typing.h"


Typing::Typing()
{
	ResourceLoading();
	//currentInd = 150;
	timeForTesting = 100;
}

void Typing::ChangeState(char letter)
{
	if (letter == '\b')
	{
		currentInd--;
		if (currentInd < 0) currentInd = 0;
		return;
	}
	if (letter != letters[currentInd].GetLetter())
	{
		letters[currentInd].SetIsCorrect(false);
		currentInd++;
		return;
	}
	else
	{
		letters[currentInd].SetIsCorrect(true);
		currentInd++;
		return;
	}
}

int Typing::GetCurrentInd()
{
	return currentInd;
}

void Typing::SetTimeForTesting(int seconds)
{
	this->timeForTesting = seconds;
}

int Typing::GetTimeForTesting()
{
	return this->timeForTesting;
}

std::vector<Letter> Typing::GetLetters()
{
	return letters;
}

void Typing::StartTyping()
{
	timer.StartTimer(timeForTesting);
}

double Typing::CheckTime()
{
	return timer.GetAvailableTime();
}

void Typing::ResourceLoading()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::vector<std::string> words;
	const char* textSource = "resources\\words.txt";

	std::ifstream textFile(textSource);
	if (!(textFile.is_open()))
	{
		return;
	}

	std::string word;
	while (std::getline(textFile, word))
	{
		words.push_back(word);
	}
	textFile.close();

	//std::shuffle(words.begin(), words.end(), rng);

	char letterSymbol;

	for (const std::string& word : words) {
		for (char letterSymbol : word) {

			Letter letter(letterSymbol);
			letters.push_back(letter);
		}
		Letter letter(' ');
		letters.push_back(letter);
	}
}



