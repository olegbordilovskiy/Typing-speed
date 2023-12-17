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

void View::PreparationUpdate(HDC hdc, RECT clientRect, int seconds)
{
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));
	SetLetterWidth(hdc);
	SetLetterHeight(hdc);
	DrawAppCondition(hdc, clientRect, preparation);
	DrawChooseTime(hdc, clientRect,seconds);
	DrawHotKeys(hdc, clientRect);
	DrawLetters(hdc, clientRect);
}
void View::TestingUpdate(HDC hdc, RECT clientRect, int seconds)
{
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));

	SetLetterWidth(hdc);
	SetLetterHeight(hdc);
	DefineNewBoundaries(hdc, clientRect);

	DrawTimer(hdc, clientRect);
	DrawChooseTime(hdc, clientRect, seconds);
	DrawHotKeys(hdc, clientRect);
	DrawLetters(hdc, clientRect);
}

void View::ResultUpdate(HDC hdc, RECT clientRect, int seconds)
{
	FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 5));
	RECT resultRect = GetNewResultRect(clientRect);
	//DrawSpaceError(hdc, resultRect);
	DrawAppCondition(hdc, clientRect, result);
	DrawResults(hdc, clientRect);
	DrawHotKeys(hdc, clientRect);


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
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT)); 
	lf.lfHeight = fontSize; 
	lf.lfWeight = FW_NORMAL; 
	lf.lfItalic = FALSE; 
	lf.lfUnderline = FALSE; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	wcscpy_s(lf.lfFaceName, L"Consolas"); 
	font = CreateFontIndirect(&lf); 
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
	case appCond:
		fontSizeCoef = 20;
		break;
	case chooseTime:
		fontSizeCoef = 30;
		break;
	case hotKeys:
		fontSizeCoef = 40;
		break;
	}

	fontSize = smallestSide / fontSizeCoef;

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

RECT View::GetNewChooseTimeRect(RECT clientRect)
{
	int clientRectWidth = clientRect.right - clientRect.left;
	int clientRectHeight = clientRect.bottom - clientRect.top;

	RECT newChooseTimeRect;
	newChooseTimeRect.right = clientRectWidth - clientRectWidth * 0.07;
	if (clientRectWidth > 600)
	{
		newChooseTimeRect.left = newChooseTimeRect.right - 180;
	}
	else
	{
		newChooseTimeRect.left = clientRectWidth - clientRectWidth / 2.8;
	}
	newChooseTimeRect.top = clientRect.top + clientRectHeight * 0.10 ;
	newChooseTimeRect.bottom = clientRect.top + clientRectHeight * 0.15;

	return newChooseTimeRect;
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

void View::DrawAppCondition(HDC hdc, RECT clientRect, appCondition condition)
{
	int clientRectWidth = clientRect.right - clientRect.left;
	int clientRectHeight = clientRect.bottom - clientRect.top;
	int appConditionX = clientRectWidth / 8;
	int appConditionY = clientRectHeight / 12;

	std::wstring conditionWSTR;
	if (condition == preparation)
	{
		conditionWSTR = L"Start typing";
	}
	else
	{
		conditionWSTR = L"Result";
	}

	SetNewFontSize(clientRect, appCond, hdc);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, appConditionX, appConditionY, conditionWSTR.c_str(), conditionWSTR.size());

}

void View::DrawTimer(HDC hdc, RECT clientRect)
{
	int clientRectWidth = clientRect.right - clientRect.left;
	int clientRectHeight = clientRect.bottom - clientRect.top;
	int numberTimerX = clientRectWidth / 8; // 2.1
	int numberTimerY = clientRectHeight / 12; // 5
	int lineTimerWidth = clientRectHeight * 0.02;
	int timeForTesting = typing->GetTimeForTesting();
	double partWidth = (double)clientRectWidth / timeForTesting;
	int partGBValue = 255 / clientRectWidth;

	int iAvailableTime = typing->CheckTime();
	float dAvailableTime = typing->CheckTime();
	int lineTimerLength = partWidth * dAvailableTime;

	int curGBValue = (255 * (dAvailableTime / timeForTesting));

	std::wstring timeWSTR = std::to_wstring(iAvailableTime + 1);
	timeWSTR += L"s";

	SetNewFontSize(clientRect, appCond, hdc);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, numberTimerX, numberTimerY, timeWSTR.c_str(), GetNumberLength(iAvailableTime) + 1);

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
		textRect.top = resultRect.top - legendHeight / 2;
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

void View::DrawChooseTime(HDC hdc, RECT clientRect, int seconds)
{
	RECT chooseTimeRect = GetNewChooseTimeRect(clientRect);
	int oneChooseTimeWidth = (chooseTimeRect.right - chooseTimeRect.left) / 4;

	SetBkColor(hdc, RGB(0, 0, 0));

	RECT timeLabelRect = chooseTimeRect;
	timeLabelRect.bottom = chooseTimeRect.top;
	timeLabelRect.top -= 30; // 
	SetNewFontSize(clientRect, chooseTime, hdc);
	DrawText(hdc, L"time", -1, &timeLabelRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	std::vector<int> timeOptions = { 15, 30, 60, 120 };

	for (int time = 0; time < 4; time++)
	{
		std::wstring timeWSTR = std::to_wstring(timeOptions[time]);

		RECT chooseOneTimeRect;
		chooseOneTimeRect.left = chooseTimeRect.left + time * oneChooseTimeWidth;
		chooseOneTimeRect.top = chooseTimeRect.top;
		chooseOneTimeRect.right = chooseTimeRect.left + (time + 1) * oneChooseTimeWidth;
		chooseOneTimeRect.bottom = chooseTimeRect.bottom;

		if (seconds == timeOptions[time])
		{
			SetTextColor(hdc, RGB(255, 255, 255));
		}
		else
		{
			SetTextColor(hdc, RGB(40, 40, 40));
		}

		DrawText(hdc, timeWSTR.c_str(), -1, &chooseOneTimeRect, DT_SINGLELINE | DT_VCENTER);
	}
}


void View::DrawHotKeys(HDC hdc, RECT clientRect)
{
	RECT enterRect;
	enterRect.left = clientRect.right - clientRect.right * 0.57;
	enterRect.top = clientRect.bottom - clientRect.bottom * 0.10;
	enterRect.right = clientRect.right - clientRect.right * 0.50;
	enterRect.bottom = clientRect.bottom - clientRect.bottom * 0.05;

	RECT restartRect;
	restartRect.left = enterRect.right;
	restartRect.top = enterRect.top;
	restartRect.right = restartRect.left+150; 
	restartRect.bottom = enterRect.bottom;

	SetNewFontSize(clientRect, hotKeys, hdc);

	SetTextColor(hdc, RGB(255, 255, 255));

	DrawEdge(hdc, &enterRect, EDGE_BUMP, BF_RECT);
	DrawText(hdc, L"Enter", -1, &enterRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	DrawText(hdc, L" - Restart", -1, &restartRect, DT_SINGLELINE | DT_VCENTER);
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

void View::SetStartTimerOption(int seconds)
{
	startTimerOption = seconds;
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

