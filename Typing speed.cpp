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
const char* path = "C:\\Users\\vanas\\OneDrive\\Рабочий стол\\3 курс\\СП\\Typing speed\\resources\\words.txt";

//BYTE ResourceLoading() {
//	ifstream wordsFile("resources\\words.txt");
//	if (!(wordsFile.is_open()))
//	{
//		return 1;
//	}
//
//	string word;
//	while (wordsFile >> word)
//	{
//		words.push_back(word);
//	}
//
//	shuffle(words.begin(), words.end(), g);
//
//	LOGFONT lf;
//	memset(&lf, 0, sizeof(LOGFONT)); // Инициализация
//	lf.lfHeight = 45; // Размер шрифта
//	lf.lfWeight = FW_NORMAL; // Вес шрифта
//	lf.lfItalic = FALSE; // Курсив
//	lf.lfUnderline = FALSE; // Подчеркивание
//	lf.lfCharSet = DEFAULT_CHARSET; // Набор символов
//	wcscpy_s(lf.lfFaceName, L"Tahoma"); // Имя шрифта
//	font = CreateFontIndirect(&lf); // Создание шрифта
//}
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
//}
//wchar_t* ConvertToWideString(const string& input) {
//	wstring wideString(input.begin(), input.end());
//	wchar_t* wideStringCopy = new wchar_t[wideString.length() + 1]; // +1 для нулевого символа
//	wcscpy_s(wideStringCopy, wideString.length() + 1, wideString.c_str());
//	return wideStringCopy;
//}
//BYTE HowManyWordsCanBeContained(HDC hdc, long distance, HFONT font, const vector<string>& words) {
//	BYTE wordsContained = 0;
//	SIZE wordSize;
//	SIZE spaceSize;
//	SIZE totalSize;
//	int charSize;
//	GetTextExtentPoint32(hdc, L" ", 1, &spaceSize);
//	//GetTextExtentPoint32(hdc, L"i", 1, &spaceSize);
//	//GetTextExtentPoint32(hdc, L"w", 1, &spaceSize);
//
//	for (const string& word : words) {
//
//		// Преобразуем строку в wchar_t*
//		wchar_t* wideWord = ConvertToWideString(word);
//
//		GetTextExtentPoint32(hdc, wideWord, wcslen(wideWord), &wordSize);
//
//		totalSize.cx = wordSize.cx + spaceSize.cx;
//
//		delete[] wideWord; // Не забудьте освободить память
//
//		if (totalSize.cx <= distance) {
//			distance -= totalSize.cx;
//			wordsContained++;
//		}
//		else {
//			break; // Не помещаются больше слов
//		}
//	}
//
//	return wordsContained;
//}
//
//wchar_t* ExtractWords(BYTE amount, const vector<string>& words, int index) {
//	wstring result;
//	BYTE wordsToExtract = amount;/* = min(amount, words.size());*/ // Учитываем, что может быть меньше слов, чем запрошено
//
//	for (BYTE i = 0; i < wordsToExtract; ++i) {
//		result += wstring(words[index + i].begin(), words[index + i].end());
//		if (i < wordsToExtract - 1) {
//			result += L' '; // Добавляем пробел между словами, кроме последнего
//		}
//	}
//
//	// Выделяем память для широкой строки и копируем в нее результат
//	wchar_t* wideResult = new wchar_t[result.length() + 1]; // +1 для нулевого символа
//	wcscpy_s(wideResult, result.length() + 1, result.c_str());
//
//
//	return wideResult;
//}
//
//
//RECT GetNewTextRect()
//{
//	RECT newTextRect;
//	float coefX = 1.2;
//	float coefY = 1.75;
//	newTextRect.left = clientRect.right - clientRect.right / coefX;
//	newTextRect.top = clientRect.bottom - clientRect.bottom / coefY;
//	newTextRect.right = clientRect.right / coefX;
//	newTextRect.bottom = clientRect.bottom / coefY;
//
//	return newTextRect;
//}
//void DrawTestingWords(HDC hdc, HFONT font, RECT drawingArea, vector<string>& words) {
//	BYTE rowsCount = 3;
//	WORD rowsHeight = (drawingArea.bottom - drawingArea.top) / rowsCount;
//	HFONT oldFont = (HFONT)SelectObject(hdc, font);
//	SIZE fontHeight;
//	SetTextColor(hdc, RGB(255, 255, 255));
//	SetBkColor(hdc, RGB(0, 0, 0));
//	GetTextExtentPoint32(hdc, L"s", 1, &fontHeight);
//	long y = drawingArea.top;
//	int wordIndex = 0;
//
//	for (BYTE i = 0; i < rowsCount; i++) {
//
//		BYTE wordsAmount = HowManyWordsCanBeContained(hdc, drawingArea.right - drawingArea.left, font, words);
//		wordIndex += wordsAmount;
//		wchar_t* TextRow = ExtractWords(wordsAmount, words, wordIndex);
//
//		TextOut(hdc, drawingArea.left, y, TextRow, lstrlen(TextRow));
//		y += rowsHeight;
//
//		// Освобождаем выделенную память после использования
//		//words.erase(words.begin(), words.begin() + wordsAmount);
//		delete[] TextRow;
//
//	}
//
//	// Восстанавливаем предыдущий шрифт
//	SelectObject(hdc, oldFont);
//}
//
//void DrawRectangle(HDC hdc)
//{
//	HBRUSH brush = CreateSolidBrush(RGB(50, 50, 50));
//	SelectObject(hdc, brush);
//	Rectangle(hdc, textRect.left, textRect.top, textRect.right, textRect.bottom);
//	DeleteObject(brush);
//}

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

	Typing* typ = new Typing(path);


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
		EndPaint(hwnd, &ps);

	}
	return 0;

	case WM_KEYDOWN:
	{
		
	}

	case WM_SIZE:
	{
		GetClientRect(hwnd, &clientRect);
		//textRect = GetNewTextRect();
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