#ifndef UNICODE
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

RECT clientRect;
RECT textRect;
PAINTSTRUCT ps;
CRITICAL_SECTION criticalSection;

typedef void (View::* ViewUpdate)(HDC hdc, RECT clientRect);

Typing* typing;
View* view;


enum ApplicationConditions
{
	preTest,
	Testing,
	Result
};

ApplicationConditions appCondition;

void AsyncCheckTimeForTestingThread()
{
	std::thread th([&]()
		{
			while (true)
			{
				EnterCriticalSection(&criticalSection);
				if (appCondition == Testing) {
					if (typing->CheckTime() == 0)
					{
						appCondition = Result;
					}
				}
				LeaveCriticalSection(&criticalSection);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
	th.detach();
}

void AsyncRequestForView(HWND hwnd) {

	std::thread th([hwnd]() {
		while (true)
		{
			InvalidateRect(hwnd, NULL, true);
			std::this_thread::sleep_for(std::chrono::milliseconds(8));
		}
		});
	th.detach();
}

//void ViewThread(HWND hwnd)
//{
//	while (true)
//	{
//		InvalidateRect(hwnd, NULL, TRUE);
//		std::this_thread::sleep_for(std::chrono::milliseconds(16));
//	}
//}
//
//void AsyncView(HWND hwnd) {
//
//	std::thread thr(ViewThread, hwnd);
//	thr.detach();
//}

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

void DoubleBuffering(HWND hwnd, ViewUpdate updateFunction, View& view)
{
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
	SelectObject(hdcBuffer, hBitmap);

	// Рисование в hdcBuffer
	//FillRect(hdcBuffer, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 5));
	//view.TestingUpdate(hdcBuffer, clientRect);
	//(view.*pViewUpdateFunc)(hdcBuffer, clientRect);
	(view.*updateFunction)(hdcBuffer, clientRect);

	// Копирование изображения из hdcBuffer в hdc
	BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

	// Очистка ресурсов
	DeleteObject(hBitmap);
	DeleteDC(hdcBuffer);

	EndPaint(hwnd, &ps);
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

	appCondition = preTest;
	InitializeCriticalSection(&criticalSection);
	typing = new Typing();
	view = new View(typing);

	GetClientRect(hwnd, &clientRect);

	ShowWindow(hwnd, SW_MAXIMIZE);

	AsyncRequestForView(hwnd);
	AsyncCheckTimeForTestingThread();

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
		EnterCriticalSection(&criticalSection);
		switch (appCondition)
		{
		case preTest:
			DoubleBuffering(hwnd, &View::TestingUpdate, *view);
			break;
		case Testing:
			DoubleBuffering(hwnd, &View::TestingUpdate, *view);
			break;
		case Result:
			DoubleBuffering(hwnd, &View::ResultUpdate, *view);
			break;
		default:
			break;
		}
		LeaveCriticalSection(&criticalSection);
	}
	return 0;

	case WM_KEYDOWN:
	{
		char lowerCase;
		EnterCriticalSection(&criticalSection);

		switch (appCondition)
		{
		case preTest:
			appCondition = Testing;
			typing->StartTyping();

		case Testing:
			lowerCase = tolower(wParam);
			typing->ChangeState(lowerCase);
			view->SetCurrentPosition(typing->GetCurrentInd());
			break;
		case Result:
			break;
		default:
			break;
		}
		LeaveCriticalSection(&criticalSection);
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
		DeleteCriticalSection(&criticalSection);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
