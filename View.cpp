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
	//DrawCurrentPosition(hdc);
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
	lf.lfHeight = 56; // ������ ������
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
	newTextRect.right = clientRect.right / (coefX-0.1);
	newTextRect.bottom = clientRect.bottom / coefY;
	return newTextRect;
}

std::wstring View::CharToWstring(char ch)
{
	wchar_t wideChar;
	MultiByteToWideChar(CP_UTF8, 0, &ch, 1, &wideChar, 1);
	return std::wstring(1, wideChar);
}

void View::DrawLetters(HDC hdc, RECT textRect)
{
	int rowsHeight = (textRect.bottom - textRect.top) / rowCount * 0.7;
	int position = startPosition;
	std::vector<Letter> letters = typing->GetLetters();
	SetBkColor(hdc, RGB(0, 0, 0));

	for (int row = 0; row < rowCount; row++)
	{
		int lettersCount = HowManyLettersCanBeContained(textRect, position);

		for (int i = 0; i < lettersCount; i++)
		{
			if (position + i <= currentPosition)
			{
				if (!letters[position + i].GetIsCorrect())
				{
					SetTextColor(hdc, RGB(255, 0, 0));
					/*if (letters[i].GetLetter() == ' ')
					{

					}*/
				}
				else
				{
					SetTextColor(hdc, RGB(255, 255, 255));
				}

				if (position + i == currentPosition)
				{
					SetBkColor(hdc, RGB(50, 50, 50));
					SetTextColor(hdc, RGB(255, 255, 255));
				}
				else
				{
					SetBkColor(hdc, RGB(0, 0, 0));
				}
			}
			else
			{
				SetTextColor(hdc, RGB(40, 40, 40));
				SetBkColor(hdc, RGB(0, 0, 0));
			}

			TextOut(hdc, textRect.left + i * letterWidth, textRect.top + row * rowsHeight,
				CharToWstring(letters[position + i].GetLetter()).c_str(), 1);
		}
		position += lettersCount;
	}
	endPosition = position;
}

void View::GetLetterWidth(HDC hdc)
{
	SelectObject(hdc, font);
	SIZE letterSize;
	GetTextExtentPoint32(hdc, L"w", 1, &letterSize);
	letterWidth = letterSize.cx;
}

void View::SetCurrentPosition(int currentInd)
{
	currentPosition = currentInd;
	DefineNewBoundaries();
}

void View::DefineNewBoundaries()
{
	if (currentPosition >= endPosition) {
		startPosition = currentPosition;
	}

}


