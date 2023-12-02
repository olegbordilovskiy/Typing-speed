#include "View.h"


View::View(Typing* typing)
{
	FontLoading();
	this->typing = typing;
	startPosition = 0;
	currentPosition = 0;
	endPosition = 0;
	rowCount = 3;	
}

void View::Update(HDC hdc, RECT clientRect)
{
	RECT textRect = GetNewTextRect(clientRect);
	//HFONT oldFont = (HFONT)SelectObject(hdc, font);

	GetLetterWidth(hdc);
	DrawLetters(hdc, textRect);




	//SelectObject(hdc, oldFont);
}



int View::HowManyLettersCanBeContained(RECT textRect, int startLetterIndex)
{
	long availableDistance = textRect.right - textRect.left;
	long occupiedDistance = 0;
	int lettersAmount = 0;
	int totalLettersAmount = 0;
	int position = startLetterIndex;
	 
	while (availableDistance > 0)
	{
		if (availableDistance - GetWordSize(position) < 0) break;
		occupiedDistance = GetWordSize(position);
		lettersAmount = occupiedDistance / letterWidth;
		availableDistance -= occupiedDistance;
		position += lettersAmount;
		totalLettersAmount += lettersAmount;
	}
	return totalLettersAmount;
}

int View::GetWordSize(int position)
{
	std::vector<Letter> letters = typing->GetLetters();
	int wordSize = 0;
	while (letters[position].GetLetter() != ' ')
	{
		wordSize += letterWidth;
		position++;
	}
	return wordSize + letterWidth;
}

void View::FontLoading()
{
	//const wchar_t* fontPath = L"C:\\Users\\vanas\\OneDrive\\������� ����\\3 ����\\��\\Typing speed\\resources\\UbuntuMono-R.ttf";

	//if (AddFontResource(fontPath) == 0)
	//{
	//	return;
	//}

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // �������������
	lf.lfHeight = 72; // ������ ������
	lf.lfWeight = FW_NORMAL; // ��� ������
	lf.lfItalic = FALSE; // ������
	lf.lfUnderline = FALSE; // �������������
	lf.lfCharSet = DEFAULT_CHARSET; // ����� ��������
	wcscpy_s(lf.lfFaceName, L"Consolas"); // �������� YourFontName �� ��� ������ ������

	// ����������� ������ ���� � ����� ������
	//wcscpy_s(lf.lfFaceName, fontPath);
	font = CreateFontIndirect(&lf); // �������� ������
}

RECT View::GetNewTextRect(RECT clientRect)
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

void View::DrawLetters(HDC hdc, RECT textRect)
{
	int rowsHeight = (textRect.bottom - textRect.top) / rowCount;
	int a = HowManyLettersCanBeContained(textRect,0);

	//SIZE fontHeight;
	//SetTextColor(hdc, RGB(255, 255, 255));
	//SetBkColor(hdc, RGB(0, 0, 0));
	//GetTextExtentPoint32(hdc, L"s", 1, &fontHeight);
	//long y = drawingArea.top;
	//int wordIndex = 0;

	//for (BYTE i = 0; i < rowsCount; i++) {

	//	BYTE wordsAmount = HowManyWordsCanBeContained(hdc, drawingArea.right - drawingArea.left, font, words);
	//	wordIndex += wordsAmount;
	//	wchar_t* TextRow = ExtractWords(wordsAmount, words, wordIndex);

	//	TextOut(hdc, drawingArea.left, y, TextRow, lstrlen(TextRow));
	//	y += rowsHeight;

	//	// ����������� ���������� ������ ����� �������������
	//	//words.erase(words.begin(), words.begin() + wordsAmount);
	//	delete[] TextRow;

	//}


}

void View::GetLetterWidth(HDC hdc)
{
	SelectObject(hdc, font);
	SIZE letterSize;
	GetTextExtentPoint32(hdc, L"w", 1, &letterSize);
	letterWidth = letterSize.cx; 
}

void View::DefineNewBoundaries(HDC hdc)
{
	if (typing->GetCurrentInd() > endPosition) {

	}
}
