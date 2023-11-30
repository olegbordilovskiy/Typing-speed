#include "View.h"


View::View(Typing* typing)
{
	FontLoading();
	this->typing = typing;
}

void View::Update(HDC hdc, RECT clientRect)
{
	RECT textRect = GetNewTextRect(clientRect);
	int rowsCount = 3;
	int rowsHeight = (drawingArea.bottom - drawingArea.top) / rowsCount;
	HFONT oldFont = (HFONT)SelectObject(hdc, font);
	SIZE fontHeight;
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkColor(hdc, RGB(0, 0, 0));
	GetTextExtentPoint32(hdc, L"s", 1, &fontHeight);
	long y = drawingArea.top;
	int wordIndex = 0;

	for (BYTE i = 0; i < rowsCount; i++) {

		BYTE wordsAmount = HowManyWordsCanBeContained(hdc, drawingArea.right - drawingArea.left, font, words);
		wordIndex += wordsAmount;
		wchar_t* TextRow = ExtractWords(wordsAmount, words, wordIndex);

		TextOut(hdc, drawingArea.left, y, TextRow, lstrlen(TextRow));
		y += rowsHeight;

		// Освобождаем выделенную память после использования
		//words.erase(words.begin(), words.begin() + wordsAmount);
		delete[] TextRow;

	}

	// Восстанавливаем предыдущий шрифт
	SelectObject(hdc, oldFont);
}

int View::HowManyWordsCanBeContained(HDC hdc)
{
	BYTE wordsContained = 0;
	SIZE wordSize;
	SIZE spaceSize;
	SIZE totalSize;
	int charSize;
	GetTextExtentPoint32(hdc, L" ", 1, &spaceSize);
	//GetTextExtentPoint32(hdc, L"i", 1, &spaceSize);
	//GetTextExtentPoint32(hdc, L"w", 1, &spaceSize);

	for (const string& word : words) {

		// Преобразуем строку в wchar_t*
		wchar_t* wideWord = ConvertToWideString(word);

		GetTextExtentPoint32(hdc, wideWord, wcslen(wideWord), &wordSize);

		totalSize.cx = wordSize.cx + spaceSize.cx;

		delete[] wideWord; // Не забудьте освободить память

		if (totalSize.cx <= distance) {
			distance -= totalSize.cx;
			wordsContained++;
		}
		else {
			break; // Не помещаются больше слов
		}
	}

	return wordsContained;
}

void View::FontLoading()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // Инициализация
	lf.lfHeight = 45; // Размер шрифта
	lf.lfWeight = FW_NORMAL; // Вес шрифта
	lf.lfItalic = FALSE; // Курсив
	lf.lfUnderline = FALSE; // Подчеркивание
	lf.lfCharSet = DEFAULT_CHARSET; // Набор символов
	wcscpy_s(lf.lfFaceName, L"Tahoma"); // Имя шрифта
	font = CreateFontIndirect(&lf); // Создание шрифта
}

RECT View::GetNewTextRect(RECT clientRect)
{
	RECT newTextRect;
	float coefX = 1.2;
	float coefY = 1.75;
	newTextRect.left = clientRect.right - clientRect.right / coefX;
	newTextRect.top = clientRect.bottom - clientRect.bottom / coefY;
	newTextRect.right = clientRect.right / coefX;
	newTextRect.bottom = clientRect.bottom / coefY;
	return newTextRect;
}
