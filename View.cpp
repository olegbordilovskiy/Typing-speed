#include "View.h"


View::View(Typing* typing)
{
	fontSize = 56;
	FontLoading();
	this->typing = typing;
	startPosition = 0;
	currentPosition = 0;
	endPosition = 0;
	rowCount = 3;
}

void View::PreparationUpdate(HDC hdc, RECT clientRect)
{
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));
	SetLetterWidth(hdc);
	SetLetterHeight(hdc);
	DrawLetters(hdc, clientRect);
}
void View::TestingUpdate(HDC hdc, RECT clientRect)
{
	//RECT textRect = GetNewTextRect(clientRect);
	//HFONT oldFont = (HFONT)SelectObject(hdc, font);
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));

	SetLetterWidth(hdc);
	SetLetterHeight(hdc);
	DefineNewBoundaries(hdc, clientRect);

	DrawTimer(hdc, clientRect);
	//DrawSpaceError(hdc, textRect);
	DrawLetters(hdc, clientRect);

	//SelectObject(hdc, oldFont);
	//DeleteObject(hBrush);
}

void View::ResultUpdate(HDC hdc, RECT clientRect)
{
	//RECT resultRect = GetNewResultRect(clientRect);
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));
	//fontSizeType type = results;
	//SetNewFontSize(clientRect, results, hdc);
	RECT resultRect = GetNewResultRect(clientRect);
	//DrawSpaceError(hdc, resultRect);
	DrawResults(hdc, clientRect);

}

int View::HowManyLettersCanBeContained(RECT textRect, int startLetterPosition, bool direction)
{
	long availableDistance = textRect.right - textRect.left;
	long occupiedDistance = 0;
	int lettersAmount = 0;
	int totalLettersAmount = 0;
	int position = startLetterPosition;
	int sign;

	if (direction) sign = 1;
	else sign = -1;

	while (availableDistance > 0)
	{
		if (position < 0 || (availableDistance - GetWordSize(position, direction) < 0)) break;
		occupiedDistance = GetWordSize(position, direction);
		lettersAmount = occupiedDistance / letterWidth;
		availableDistance -= occupiedDistance;
		position += lettersAmount * sign;
		totalLettersAmount += lettersAmount;
	}
	return totalLettersAmount;
}

int View::GetWordSize(int position, bool direction)
{
	std::vector<Letter> letters = typing->GetLetters();
	int sign;
	int wordSize = 0;

	if (direction) sign = 1;
	else sign = -1;

	while (position >= 0 && letters[position].GetLetter() != ' ')
	{
		wordSize += letterWidth;
		position += sign;
	}
	return wordSize + letterWidth;
}

void View::FontLoading()
{
	//const wchar_t* fontPath = L"C:\\Users\\vanas\\OneDrive\\Рабочий стол\\3 курс\\СП\\Typing speed\\resources\\UbuntuMono-R.ttf";

	//if (AddFontResource(fontPath) == 0)
	//{
	//	return;
	//}

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); // Инициализация
	lf.lfHeight = fontSize; // Размер шрифта
	lf.lfWeight = FW_NORMAL; // Вес шрифта
	lf.lfItalic = FALSE; // Курсив
	lf.lfUnderline = FALSE; // Подчеркивание
	lf.lfCharSet = DEFAULT_CHARSET; // Набор символов
	wcscpy_s(lf.lfFaceName, L"Consolas"); // Замените YourFontName на имя вашего шрифта

	// Используйте полный путь к файлу шрифта
	//wcscpy_s(lf.lfFaceName, fontPath);
	font = CreateFontIndirect(&lf); // Создание шрифта
}

void View::SetNewFontSize(RECT clientRect, fontSizeType type, HDC& hdc)
{
	int clientRectWidth = clientRect.right - clientRect.left;
	int clientRectHeight = clientRect.bottom - clientRect.top;

	int smallestSide = min(clientRectWidth, clientRectHeight);
	int fontSizeCoef = 18;

	switch (type)
	{
	case text:
		fontSizeCoef = 18;
		break;
	case results:
		fontSizeCoef = 8;
		break;
	case resultsLabels:
		fontSizeCoef = 20;
		break;
	}

	fontSize = smallestSide / fontSizeCoef;

	// Измените размер текущего шрифта
	LOGFONT lf;
	GetObject(font, sizeof(LOGFONT), &lf);
	lf.lfHeight = fontSize;
	DeleteObject(font);
	font = CreateFontIndirect(&lf);
	SelectObject(hdc, font);
}


RECT View::GetNewTextRect(RECT clientRect)
{
	RECT newTextRect;
	float coefX = 1.2;
	float coefY = 1.7;
	newTextRect.left = clientRect.right - clientRect.right / coefX;
	newTextRect.top = clientRect.bottom - clientRect.bottom / coefY;
	newTextRect.right = clientRect.right / (coefX - 0.1);
	newTextRect.bottom = clientRect.bottom / coefY;
	return newTextRect;
}

RECT View::GetNewResultRect(RECT clientRect)
{
	RECT newResultRect;
	float coefX = 1.27;
	float coefY = 1.6;
	newResultRect.left = clientRect.right - clientRect.right / coefX;
	newResultRect.top = clientRect.bottom - clientRect.bottom / coefY;
	newResultRect.right = clientRect.right / coefX;
	newResultRect.bottom = clientRect.bottom / coefY;
	return newResultRect;
}


std::wstring View::CharToWstring(char ch)
{
	wchar_t wideChar;
	MultiByteToWideChar(CP_UTF8, 0, &ch, 1, &wideChar, 1);
	return std::wstring(1, wideChar);
}

void View::DrawLetters(HDC hdc, RECT clientRect)
{
	RECT textRect = GetNewTextRect(clientRect);
	//fontSizeType type = text;
	SetNewFontSize(clientRect, text, hdc);
	int rowsHeight = (textRect.bottom - textRect.top) / rowCount; // 0.7
	int position = startPosition;
	bool isThisSpaceError = false;
	std::vector<Letter> letters = typing->GetLetters();
	//SetBkColor(hdc, RGB(0, 0, 0));

	for (int row = 0; row < rowCount; row++)
	{
		int lettersCount = HowManyLettersCanBeContained(textRect, position, true);

		for (int i = 0; i < lettersCount; i++)
		{
			if (position + i < currentPosition)
			{
				if (!letters[position + i].GetIsCorrect())
				{
					if (letters[position + i].GetLetter() == ' ')
					{
						isThisSpaceError = true;
					}
					else
					{
						SetTextColor(hdc, RGB(255, 0, 0));
					}
				}
				else
				{
					SetTextColor(hdc, RGB(255, 255, 255));
				}

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

			if (position + i > currentPosition)
			{
				SetTextColor(hdc, RGB(40, 40, 40));
				SetBkColor(hdc, RGB(0, 0, 0));
			}

			TextOut(hdc, textRect.left + i * letterWidth, textRect.top + row * rowsHeight,
				CharToWstring(letters[position + i].GetLetter()).c_str(), 1);

			if (isThisSpaceError)
			{
				RECT spaceErrorRect;
				double spaceErrorLineHeight = GetSpaceErrorLineHeight();
				spaceErrorRect.left = textRect.left + letterWidth * i;
 				spaceErrorRect.top = textRect.top + row * rowsHeight + letterHeight * spaceErrorLineHeight;
				spaceErrorRect.right = spaceErrorRect.left + letterWidth;
				spaceErrorRect.bottom = textRect.top + row * rowsHeight + letterHeight;
				DrawSpaceError(hdc, spaceErrorRect);
				isThisSpaceError = false;
			}
		}
		position += lettersCount;
	}
	endPosition = position - 1;
}

void View::DrawTimer(HDC hdc, RECT clientRect)
{
	int clientRectWidth = clientRect.right - clientRect.left;
	int clientRectHeight = clientRect.bottom - clientRect.top;
	int numberTimerX = clientRectWidth / 7; // 2.1
	int numberTimerY = clientRectHeight / 10; // 5
	int lineTimerWidth = clientRectHeight * 0.02;
	int timeForTesting = typing->GetTimeForTesting();
	float partWidth = (float)clientRectWidth / timeForTesting;
	int partGBValue = 255 / clientRectWidth;

	int iAvailableTime = typing->CheckTime();
	float dAvailableTime = typing->CheckTime();
	int lineTimerLength = partWidth * dAvailableTime;

	int curGBValue = (255 * (dAvailableTime / timeForTesting));

	std::wstring timeWStr = std::to_wstring(iAvailableTime);
	timeWStr += L"s";

	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, numberTimerX, numberTimerY, timeWStr.c_str(), GetNumberLength(iAvailableTime) + 1);

	hBrush = CreateSolidBrush(RGB(255, curGBValue, curGBValue));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, clientRect.left, clientRect.top, lineTimerLength, lineTimerWidth);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}

void View::DrawResults(HDC hdc, RECT clientRect)
{
	RECT resultRect = GetNewResultRect(clientRect);
	int resultsWidth = resultRect.right - resultRect.left;
	int resultsHeight = resultRect.bottom - resultRect.top;
	int oneResultWidth = resultsWidth / 3;
	int legendHeight = resultsHeight / 3;

	std::vector<int> result = { typing->GetWPM(), typing->GetAccuracy(), typing->GetTotalTime() };

	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));

	std::vector<std::wstring> labels = { L"WPM", L"Accuracy", L"Time" };

	for (int column = 0; column < 3; column++)
	{
		std::wstring resultWSTR = std::to_wstring(result[column]);
		std::wstring label = labels[column];

		if (label == L"Time")
			resultWSTR += L"s";
		else if (label == L"Accuracy")
			resultWSTR += L"%";

		RECT textRect;
		textRect.left = resultRect.left + column * oneResultWidth;
		textRect.top = resultRect.top - legendHeight; 
		textRect.right = resultRect.left + (column + 1) * oneResultWidth;
		textRect.bottom = resultRect.top; 

		SetNewFontSize(clientRect, resultsLabels, hdc);
		DrawText(hdc, label.c_str(), -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		textRect.top = resultRect.top - legendHeight;
		textRect.bottom = resultRect.bottom;

		SetNewFontSize(clientRect, results, hdc);
		DrawText(hdc, resultWSTR.c_str(), -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}



void View::DrawSpaceError(HDC hdc, RECT spaceErrorRect)
{
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, spaceErrorRect.left, spaceErrorRect.top, spaceErrorRect.right, spaceErrorRect.bottom);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}

void View::SetLetterWidth(HDC hdc)
{
	SelectObject(hdc, font);
	SIZE letterSize;
	GetTextExtentPoint32(hdc, L"w", 1, &letterSize);
	letterWidth = letterSize.cx;
}

void View::SetLetterHeight(HDC hdc)
{
	SelectObject(hdc, font);
	SIZE letterSize;
	GetTextExtentPoint32(hdc, L"w", 1, &letterSize);
	letterHeight = letterSize.cy;
}

double View::GetSpaceErrorLineHeight()
{
	double spaceErrorLineHeight = 0.9;
	if (fontSize < 22) spaceErrorLineHeight = 0.87;
	if (fontSize < 17) spaceErrorLineHeight = 0.82;
	if (fontSize < 12) spaceErrorLineHeight = 0.77;
	return spaceErrorLineHeight;
}

int View::GetNumberLength(int number)
{
	std::string numberString = std::to_string(number);
	int length = static_cast<int>(numberString.length());
	return length;
}

void View::SetCurrentPosition(int currentInd)
{
	currentPosition = currentInd;
}

void View::SetNewStartPosition(HDC hdc, RECT textRect)
{
	int position = typing->GetCurrentInd();

	for (int row = rowCount; row > 0; row--)
	{
		position -= HowManyLettersCanBeContained(textRect, position, false);

	}
	startPosition = position + 2;
	if (startPosition == 1) startPosition = 0;
}

void View::DefineNewBoundaries(HDC hdc, RECT clientRect)
{
	RECT textRect = GetNewTextRect(clientRect);

	if (currentPosition > endPosition)
	{
		startPosition = currentPosition;
	}

	if (currentPosition < startPosition)
	{
		SetNewStartPosition(hdc, textRect);
	}
}

