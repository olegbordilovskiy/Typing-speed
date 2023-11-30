#pragma once
#include "Typing.h"
#include <windows.h>
#include <windowsx.h>
class View
{
private:
	int HowManyWordsCanBeContained(HDC hdc);
	void FontLoading();
	RECT GetNewTextRect(RECT clientRect);
public:
	View(Typing* typing);
	void Update(HDC hdc, RECT clientRect);

	Typing* typing;
	HFONT font;
	RECT clientRect;
	HWND hwnd;
};

