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
	void GetLetterWidth(HDC hdc);
	void DefineNewBoundaries(HDC hdc);
	void DrawCurrentPosition(HDC hdc);
	RECT GetNewTextRect(RECT clientRect);
	std::wstring CharToPWChar(char ch);

	int startPosition;
	int currentPosition;
	int endPosition;
	int letterWidth;
	int rowCount;
	Typing* typing;
	HFONT font;
	RECT clientRect;

public:
	View(Typing* typing);
	void Update(HDC hdc, RECT clientRect);
};

