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
		appCond,
		chooseTime,
		hotKeys,
	};

	enum appCondition
	{
		preparation,
		result,
	};

	int startPosition;
	int startTimerOption;
	int currentPosition;
	int endPosition;
	int letterWidth;
	int letterHeight;
	int rowCount;
	int fontSize;
	int resultsFontSize;
	int currentTimeOption;

	Typing* typing;
	HFONT font;
	RECT clientRect;
	HBRUSH hBrush;

	int HowManyLettersCanBeContained(RECT textRect, int startLetterIndex, bool direction);
	int GetWordSize(int position, bool direction);
	void FontLoading();
	void DrawLetters(HDC hdc, RECT clientRect);
	void DrawAppCondition(HDC hdc, RECT clientRect, appCondition condition);
	void DrawTimer(HDC hdc, RECT clientRect);
	void DrawResults(HDC hdc, RECT clientRect);
	void DrawChooseTime(HDC hdc, RECT clientRect);
	void DrawSpaceError(HDC hdc, RECT spaceErrorRect);
	void DrawHotKeys(HDC hdc, RECT clientRect);
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
	void SetStartTimeOption(int seconds);
	void SetCurrentTimeOption(int seconds);
	RECT GetNewChooseTimeRect(RECT clientRect);
};

