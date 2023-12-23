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
typedef void (View::* ViewUpdate)(HDC hdc, RECT clientRect);

RECT clientRect;
PAINTSTRUCT ps;
CRITICAL_SECTION criticalSection;

const int FPS_120 = 8;
const int DELAY_FOR_REST = 10;

Typing* typing;
View* view;

bool runCheckTime = false;
bool runAppConditions = false;
int currentTimeOption = 15;

enum ApplicationConditions
{
	preparation,
	inputWaiting,
	startTesting,
	testing,
	restart,
	result,
	final
};

ApplicationConditions appCondition;

void AsyncApplicationCondition()
{
	std::thread th([&]()
		{
			while (true) {
				if (runAppConditions) {
					EnterCriticalSection(&criticalSection);

					switch (appCondition)
					{
					case preparation:
					{
						typing = new Typing();
						view = new View(typing);
					}

					view->SetStartTimeOption(currentTimeOption);
					view->SetCurrentTimeOption(currentTimeOption);
					typing->SetTimeForTesting(currentTimeOption);

					appCondition = inputWaiting;
					break;

					case startTesting:
					{
						runCheckTime = true;
						typing->StartTyping();
						appCondition = testing;
						break;
					}

					case restart:

						runCheckTime = false;
						delete typing;
						delete view;
						appCondition = preparation;

						break;
					case final:
						runCheckTime = false;
						delete typing;
						delete view;
					default:
						break;
					}
				}
				LeaveCriticalSection(&criticalSection);
				std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_FOR_REST));
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
				if (runCheckTime) {
					EnterCriticalSection(&criticalSection);
					if (appCondition == testing) {
						if (typing->CheckTime() == 0)
						{
							appCondition = result;
						}
					}
					LeaveCriticalSection(&criticalSection);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_FOR_REST));
			}
		});
	th.detach();
}

void AsyncRequestForView(HWND hwnd) {

	std::thread th([hwnd]() {
		while (true)
		{
			InvalidateRect(hwnd, NULL, true);
			std::this_thread::sleep_for(std::chrono::milliseconds(FPS_120));
		}
		});
	th.detach();
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
		0,
		CLASS_NAME,
		L"Typing speed",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, 1500, 800,

		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	appCondition = preparation;
	runAppConditions = true;
	runCheckTime = true;

	InitializeCriticalSection(&criticalSection);

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

			lowerCase = tolower(wParam);

			if (lowerCase == '\r')
			{
				appCondition = restart;
				break;
			}

			if (IsCharAlphaNumeric(lowerCase) || lowerCase == ' ' || lowerCase == '\b')
			{
				typing->SetTimeForTesting(currentTimeOption);
				appCondition = startTesting;
			}
			else break;


		case testing:
			lowerCase = tolower(wParam);

			if (lowerCase == '\r')
			{
				appCondition = restart;
				break;
			}

			if (IsCharAlphaNumeric(lowerCase) || lowerCase == ' ' || lowerCase == '\b')
			{
				typing->ChangeState(lowerCase);
				view->SetCurrentPosition(typing->GetCurrentInd());
			}
			break;
		case result:
			lowerCase = tolower(wParam);
			if (lowerCase == '\r')
			{
				appCondition = restart;
				break;
			}
			break;
		default:
			break;
		}
		LeaveCriticalSection(&criticalSection);
	}

	case WM_LBUTTONDOWN:
	{
		if (appCondition == inputWaiting || appCondition == testing)
		{
			int mouseX = GET_X_LPARAM(lParam);
			int mouseY = GET_Y_LPARAM(lParam);

			RECT chooseTimeRect = view->GetNewChooseTimeRect(clientRect);
			std::vector<int> timeOptions = { 15, 30, 60, 120 };
			int oneTimeWidth = (chooseTimeRect.right - chooseTimeRect.left) / 4;

			for (int time = 0; time < 4; time++) {

				RECT currentTimeRect;
				currentTimeRect.left = chooseTimeRect.left + time * oneTimeWidth;
				currentTimeRect.top = chooseTimeRect.top;
				currentTimeRect.right = chooseTimeRect.left + (time + 1) * oneTimeWidth;
				currentTimeRect.bottom = chooseTimeRect.bottom;

				if (PtInRect(&currentTimeRect, { mouseX, mouseY }))
				{

					currentTimeOption = timeOptions[time];
					view->SetCurrentTimeOption(currentTimeOption);
				}
			}

		}
		return 0;
	}

	case WM_SIZE:
	{
		GetClientRect(hwnd, &clientRect);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}

	case WM_DESTROY:
		EnterCriticalSection(&criticalSection);
		runAppConditions = false;
		runCheckTime = false;
		appCondition = final;
		LeaveCriticalSection(&criticalSection);
		PostQuitMessage(0);
		DeleteCriticalSection(&criticalSection);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
