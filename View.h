#pragma once
#include "Typing.h"
#include <windows.h>
#include <windowsx.h>
#include <string>

class View
{
private:
	enum fontSizeType
	{
		text,
		results,
		resultsLabels,
	};

	int startPosition;
	int currentPosition;
	int endPosition;
	int letterWidth;
	int letterHeight;
	int rowCount;
	int fontSize;
	int resultsFontSize;

	Typing* typing;
	HFONT font;
	RECT clientRect;
	HBRUSH hBrush;

	int HowManyLettersCanBeContained(RECT textRect, int startLetterIndex, bool direction);
	int GetWordSize(int position, bool direction);
	void FontLoading();
	void DrawLetters(HDC hdc, RECT clientRect);
	void DrawTimer(HDC hdc, RECT clientRect);
	void DrawResults(HDC hdc, RECT clientRect);
	void DrawSpaceError(HDC hdc, RECT spaceErrorRect);
	void SetLetterWidth(HDC hdc);
	void SetLetterHeight(HDC hdc);
	int GetNumberLength(int number);
	void SetNewStartPosition(HDC hdc, RECT textRect);
	void SetNewFontSize(RECT clientRect, fontSizeType type, HDC& hdc);
	void DefineNewBoundaries(HDC hdc, RECT textRect);
	double GetSpaceErrorLineHeight();
	RECT GetNewTextRect(RECT clientRect);
	RECT GetNewResultRect(RECT clientRect);
	std::wstring CharToWstring(char ch);

public:
	View(Typing* typing);
	void PreparationUpdate(HDC hdc, RECT clientRect);
	void TestingUpdate(HDC hdc, RECT clientRect);
	void ResultUpdate(HDC hdc, RECT clientRect);
	void SetCurrentPosition(int currentInd);
};

