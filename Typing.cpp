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
	//currentInd++;
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
	std::random_device rd;
	std::mt19937 rng(rd());
	std::vector<std::string> words;
	const char* textSource = "C:\\Users\\vanas\\OneDrive\\Рабочий стол\\3 курс\\СП\\Typing speed\\resources\\words.txt";

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



