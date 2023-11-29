#pragma once
class Letter
{
public:
	Letter(char letter) {
		this->letter = letter;
		this->isCorrect = true;
	}
	char GetLetter() {
		return this->letter;
	}
	bool GetIsCorrect() {
		return this->isCorrect;
	}
private:
	char letter;
	bool isCorrect;
};

