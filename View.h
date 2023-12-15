#pragma once
#include "Typing.h"
#include <windows.h>
#include <windowsx.h>
#include <string>

class View
{
private:
	int HowManyLettersCanBeContained(RECT textRect, int startLetterIndex, bool direction);
	int GetWordSize(int position, bool direction);
	void FontLoading();
	void DrawLetters(HDC hdc, RECT clientRect);
	void DrawTimer(HDC hdc, RECT clientRect);
	void DrawSpaceError(HDC hdc, RECT spaceErrorRect);
	void GetLetterWidth(HDC hdc);
	int GetNumberLength(int number);
	void SetNewStartPosition(HDC hdc, RECT textRect);
	void DefineNewBoundaries(HDC hdc, RECT textRect);
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
	//PAINTSTRUCT ps;
	HBRUSH hBrush;

public:
	View(Typing* typing);
	void TestingUpdate(HDC hdc, RECT clientRect);
	void ResultUpdate(HDC hdc, RECT clientRect);
	void SetCurrentPosition(int currentInd);
};

