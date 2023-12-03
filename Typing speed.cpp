#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include "Resource.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <algorithm>
#include <random>
#include "Typing.h"
#include "View.h"
#include <cctype>

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
PAINTSTRUCT ps;
RECT clientRect;
RECT textRect;
//vector<string> words;
//vector <string> currentWords;
HFONT font;
int currentTextPosition;
random_device random;
mt19937 g(random());
//View view;

Typing* typ = new Typing();
View view(typ);

RECT GetNewTextRect(RECT clientRect)
{
	RECT newTextRect;
	float coefX = 1.2;
	float coefY = 1.6;
	newTextRect.left = clientRect.right - clientRect.right / coefX;
	newTextRect.top = clientRect.bottom - clientRect.bottom / coefY;
	newTextRect.right = clientRect.right / coefX;
	newTextRect.bottom = clientRect.bottom / coefY;
	return newTextRect;
}



//	shuffle(words.begin(), words.end(), g);

//void FontResize(HFONT& font, RECT drawingArea)
//{
//	WORD size = 0;
//	WORD fontSize = 0;
//
//	if (drawingArea.bottom - drawingArea.top > drawingArea.right - drawingArea.left)
//	{
//		size = (drawingArea.bottom - drawingArea.top) / 3;
//		fontSize = size % 72;
//	}
//	else
//	{
//		size = drawingArea.right - drawingArea.left;
//		fontSize = size % 36;
//	}
//
//	WORD totalSize = drawingArea.bottom - drawingArea.top;
//	WORD rowHeight = totalSize / 3;
//	WORD fontSize = rowHeight % 72;
//	LOGFONT lf;
//	memset(&lf, 0, sizeof(LOGFONT)); // Инициализация
//	lf.lfHeight = fontSize; // Размер шрифта
//	lf.lfWeight = FW_NORMAL; // Вес шрифта
//	lf.lfItalic = FALSE; // Курсив
//	lf.lfUnderline = FALSE; // Подчеркивание
//	lf.lfCharSet = DEFAULT_CHARSET; // Набор символов
//	wcscpy_s(lf.lfFaceName, L"Tahoma"); // Имя шрифта
//	font = CreateFontIndirect(&lf); // Создание шрифта
//

void DrawRectangle(HDC hdc)
{
	HBRUSH brush = CreateSolidBrush(RGB(50, 50, 50));
	SelectObject(hdc, brush);
	Rectangle(hdc, textRect.left, textRect.top, textRect.right, textRect.bottom);
	DeleteObject(brush);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASSEX wcex = { };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = CLASS_NAME;

	RegisterClassEx(&wcex);

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Typing speed",				// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 1500, 800,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	GetClientRect(hwnd, &clientRect);

	ShowWindow(hwnd, SW_MAXIMIZE);
	//ShowWindow(hwnd, nCmdShow);

	
	
	


	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 5));
		//DrawRectangle(hdc);
		//DrawTestingWords(hdc, font, textRect, words);

		view.Update(hdc, clientRect);

		//view.Update(hdc, clientRect);
		EndPaint(hwnd, &ps);

	}
	return 0;

	case WM_KEYDOWN:
	{
		char lowerCase = tolower(wParam);
		//char lowerCase = wParam;
		typ->ChangeState(lowerCase);
		
		InvalidateRect(hwnd, NULL, TRUE);
	}

	case WM_SIZE:
	{
		GetClientRect(hwnd, &clientRect);
		textRect = GetNewTextRect(clientRect);
		//FontResize(font, textRect);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}