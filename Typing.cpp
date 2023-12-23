#include "Typing.h"


Typing::Typing()
{
	ResourceLoading();
	timeForTesting = 15;
	currentInd = 0;
}

void Typing::ChangeState(char symbol)
{
	if (symbol == '\b')
	{
		currentInd--;
		if (currentInd < 0) currentInd = 0;
		return;
	}
	if (symbol != letters[currentInd].GetLetter())
	{
		letters[currentInd].SetIsCorrect(false);
		currentInd++;
	}
	else
	{
		letters[currentInd].SetIsCorrect(true);
		currentInd++;
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

float Typing::CheckTime()
{
	return timer.GetAvailableTime();
}

int Typing::GetWPM()
{
	int passedTime = timer.GetTotalTime() - timer.GetAvailableTime();
	double passedTimeMinutes = passedTime / 60.0;
	if (passedTimeMinutes == 0.0) return 0;
	int WPM = currentInd / (5 * passedTimeMinutes);
	return WPM;
}

int Typing::GetAccuracy()
{
	int correctLetters = 0;

	if (currentInd == 0)
	{
		return 0;
	}

	for (int i = 0; i < currentInd; i++)
	{
		if (letters[i].GetIsCorrect())
			correctLetters++;
	}

	int accuracy = ((float)correctLetters / (float)currentInd) * 100;
	return accuracy;
}

int Typing::GetTotalTime()
{
	return timer.GetTotalTime();
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

	std::shuffle(words.begin(), words.end(), rng);

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



