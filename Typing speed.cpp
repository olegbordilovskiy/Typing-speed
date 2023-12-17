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

bool runCheckTime = false;
bool runAsyncRequestForView = false;
bool isThisFirstTest = true;


enum ApplicationConditions
{
	preparation,
	inputWaiting,
	startTesting,
	testing,
	restart,
	result
};

ApplicationConditions appCondition;


void AsyncApplicationCondition()
{
	std::thread th([&]()
		{
			while (true) {
				EnterCriticalSection(&criticalSection);
				switch (appCondition)
				{
				case preparation:
				{
					typing = new Typing();
					view = new View(typing);
				}
				appCondition = inputWaiting;
				break;

				case startTesting:
				{
					runCheckTime = true;
					typing->StartTyping();
					appCondition = testing;
					break;
				}
				case result:
					break;

				case restart:

					runCheckTime = false;
					delete typing;
					delete view;
					isThisFirstTest = false;
					appCondition = preparation;

					break;
				default:
					break;
				}
				LeaveCriticalSection(&criticalSection);
			}
		});
	th.detach();
}

void AsyncCheckTimeForTestingThread()
{
	std::thread th([&]()
		{
			while (true)
			{
				EnterCriticalSection(&criticalSection);
				if (runCheckTime && appCondition == testing) {
					if (typing->CheckTime() == 0)
					{
						appCondition = result;
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

	(view.*updateFunction)(hdcBuffer, clientRect);

	BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(hdcBuffer);

	EndPaint(hwnd, &ps);
}

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

	appCondition = preparation;

	InitializeCriticalSection(&criticalSection);

	//typing = new Typing();
	//view = new View(typing);

	GetClientRect(hwnd, &clientRect);

	ShowWindow(hwnd, SW_MAXIMIZE);

	AsyncApplicationCondition();
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
		case inputWaiting:
			DoubleBuffering(hwnd, &View::PreparationUpdate, *view);
			break;
		case testing:
			DoubleBuffering(hwnd, &View::TestingUpdate, *view);
			break;
		case result:
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
		case inputWaiting:

			appCondition = startTesting;

		case testing:
			lowerCase = tolower(wParam);

			if (lowerCase == '\r')
			{
				appCondition = restart;
				break;
			}

			typing->ChangeState(lowerCase);
			view->SetCurrentPosition(typing->GetCurrentInd());
			break;
		case result:
			break;
		default:
			break;
		}
		LeaveCriticalSection(&criticalSection);
	}

	case WM_SIZE:
	{
		GetClientRect(hwnd, &clientRect);
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
