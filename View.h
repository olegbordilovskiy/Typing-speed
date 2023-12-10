#pragma once
#include "Typing.h"
#include <windows.h>
#include <windowsx.h>
#include <string>

class View
{
private:
	int HowManyLettersCanBeContained(RECT textRect, int startLetterIndex);
	int GetWordSize(int position);
	void FontLoading();
	void DrawLetters(HDC hdc, RECT clientRect);
	void DrawTimer(HDC hdc, RECT clientRect);
	void GetLetterWidth(HDC hdc);
	int GetNumberLength(int number);
	void DefineNewBoundaries();
	RECT GetNewTextRect(RECT clientRect);
	std::wstring CharToWstring(char ch);

	int startPosition;
	int currentPosition;
	int endPosition;
	int letterWidth;
	int rowCount;
	Typing* typing;
	HFONT font;
	RECT clientRect;
	PAINTSTRUCT ps;

public:
	View(Typing* typing);
	void TestingUpdate(HDC hdc, RECT clientRect);
	void SetCurrentPosition(int currentInd);
};
