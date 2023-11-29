#pragma once
class Letter
{
public:
	Letter(wchar_t letter) {
		this->letter = letter;
		this->isCorrect = true;
	}
	wchar_t GetLetter() {
		return this->letter;
	}
	bool GetIsCorrect() {
		return this->isCorrect;
	}
private:
	wchar_t letter;
	bool isCorrect;
};

