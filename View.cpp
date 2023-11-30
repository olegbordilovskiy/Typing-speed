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

		// ����������� ���������� ������ ����� �������������
		//words.erase(words.begin(), words.begin() + wordsAmount);
		delete[] TextRow;

	}

	// ��������������� ���������� �����
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

		// ����������� ������ � wchar_t*
		wchar_t* wideWord = ConvertToWideString(word);

		GetTextExtentPoint32(hdc, wideWord, wcslen(wideWord), &wordSize);

		totalSize.cx = wordSize.cx + spaceSize.cx;

		delete[] wideWord; // �� �������� ���������� ������

		if (totalSize.cx <= distance) {
			distance -= totalSize.cx;
			wordsContained++;
		}
		else {
			break; // �� ���������� ������ ����
		}
	}

	return wordsContained;
}

void View::FontLoading()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // �������������
	lf.lfHeight = 45; // ������ ������
	lf.lfWeight = FW_NORMAL; // ��� ������
	lf.lfItalic = FALSE; // ������
	lf.lfUnderline = FALSE; // �������������
	lf.lfCharSet = DEFAULT_CHARSET; // ����� ��������
	wcscpy_s(lf.lfFaceName, L"Tahoma"); // ��� ������
	font = CreateFontIndirect(&lf); // �������� ������
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
