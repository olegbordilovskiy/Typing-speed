#pragma once
#include "Typing.h"
#include <windows.h>
#include <windowsx.h>
class View
{
private:
	int HowManyLettersCanBeContained(RECT textRect, int startLetterIndex);
	int GetWordSize(int position);
	void FontLoading();
	void DrawLetters(HDC hdc, RECT clientRect);
	void GetLetterWidth(HDC hdc);
	void DefineNewBoundaries(HDC hdc);
	RECT GetNewTextRect(RECT clientRect);

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

