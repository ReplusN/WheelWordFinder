#include "WordWheel.h"
#include <fstream>
#include <math.h>

WordWheel::WordWheel(const std::string& WheelFile)
{
	std::ifstream InWheel(WheelFile);
	if (!InWheel.is_open())
	{
		throw(std::exception("WheelFile not found."));
		return;
	}
	int WheelNum = 0, LetterNum = 0;
	InWheel >> WheelNum >> LetterNum;
	if (WheelNum <= 0 || LetterNum <= 0)
	{
		throw(std::exception("WheelFile header invalid."));
		return;
	}
	mWheelNum = WheelNum;
	mLetterNum = LetterNum;

	mWheelData.resize(mWheelNum);
	for (uint32_t i = 0; i < mWheelNum; i++)
	{
		std::string WheelStr;
		InWheel >> WheelStr;
		if (WheelStr.length() != mLetterNum)
			throw(std::exception("Wheel line invalid."));
		mWheelData[i] = WheelStr;
	}
	InWheel.close();
}

WordWheel::~WordWheel()
{
}

uint32_t WordWheel::GetLetterNum()
{
	return mLetterNum;
}

uint32_t WordWheel::GetWheelNum()
{
	return mWheelNum;
}

size_t WordWheel::GetComboNum()
{
	return pow(mLetterNum,mWheelNum);
}

size_t WordWheel::GetComboGroupNum()
{
	return pow(mLetterNum, mWheelNum - 1);
}

std::string& WordWheel::operator[](size_t Index)
{
	return mWheelData[Index];
}

std::string& WordWheel::GetWheel(size_t Index)
{
	return mWheelData[Index];
}

std::string WordWheel::GetCombo(size_t Index)
{
	std::string ComboStr = "";
	ComboStr.resize(mWheelNum);
	size_t NextIndex = Index;
	for (int i = mWheelNum - 1; i >= 0; i--)
	{
		size_t CurIndex = NextIndex % mLetterNum;
		NextIndex /= mLetterNum;
		ComboStr[i] = mWheelData[i][CurIndex];
	}
	return ComboStr;
}
