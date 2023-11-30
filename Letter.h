#pragma once
class Letter
{
public:
	Letter(char letter);
	char GetLetter();
	void SetIsCorrect(bool isCorrect);
	bool GetIsCorrect();
private:
	char letter;
	bool isCorrect;
};

