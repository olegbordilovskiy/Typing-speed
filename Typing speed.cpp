﻿#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "Typing.h"
#include "View.h"
#include <cctype>
#include <thread>

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
RECT GetNewTextRect(RECT clientRect);

PAINTSTRUCT ps;
RECT clientRect;
RECT textRect;
HFONT font;

Typing* typ = new Typing();
Timer* timer = new Timer();
View view(typ);

void ViewThread(HWND hwnd)
{
	while (true)
	{
		InvalidateRect(hwnd, NULL, TRUE);
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

void AsyncView(HWND hwnd) {

	std::thread thr(ViewThread, hwnd);
	thr.detach();
}

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
	
	AsyncView(hwnd);

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

		// Создаем буфер
		HDC hdcBuffer = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(hdcBuffer, hBitmap);

		// Рисование в hdcBuffer
		FillRect(hdcBuffer, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 5));
		view.Update(hdcBuffer, clientRect);

		// Копирование изображения из hdcBuffer в hdc
		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

		// Очистка ресурсов
		DeleteObject(hBitmap);
		DeleteDC(hdcBuffer);

		EndPaint(hwnd, &ps);

	}
	return 0;

	case WM_KEYDOWN:
	{
		//timer->StartTimer(60);
		char lowerCase = tolower(wParam);
		typ->ChangeState(lowerCase);
		view.SetCurrentPosition(typ->GetCurrentInd());
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
