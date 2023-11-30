#include "Letter.h"

Letter::Letter(char letter) {
	this->letter = letter;
	this->isCorrect = true;
}
char Letter::GetLetter() {
	return this->letter;
}
void Letter::SetIsCorrect(bool isCorrect)
{
	this->isCorrect = isCorrect;
}
bool Letter::GetIsCorrect() {
	return this->isCorrect;
}
