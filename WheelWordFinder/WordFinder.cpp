#include "WordFinder.h"
#include <iostream>
#include <thread>

WordFinder::WordFinder(const WordWheelPtr& InWheel, const WordDictionaryPtr& InDict)
	: mWheel(InWheel)
	, mDict(InDict)
{
	mFoundMtx.reset(new std::mutex());
	mLevelMtx.reset(new std::mutex());
}

WordFinder::~WordFinder()
{
}

void WordFinder::FindWords()
{
	mWordsFound.clear();

	size_t ComboGroupNum = mWheel->GetComboGroupNum();
	for (size_t ComboGroupIndex = 0; ComboGroupIndex < ComboGroupNum; ComboGroupIndex++)
	{
		size_t ComboGroupStart = ComboGroupIndex * mWheel->GetLetterNum();
		std::string CurCombo = mWheel->GetCombo(ComboGroupStart);
		for (size_t StrStart = 0; StrStart < CurCombo.length() - 1; StrStart++)
		{
			for (size_t StrLength = 2; StrLength <= CurCombo.length() - StrStart; StrLength++)
			{
				std::string SubStr = CurCombo.substr(StrStart, StrLength);
				if (mDict->IsWordValid(SubStr))
				{
					mWordsFound.insert(WheelWord(SubStr, StrStart));
				}
			}
		}

		for (size_t ComboIndex = ComboGroupStart + 1; ComboIndex < ComboGroupStart + mWheel->GetLetterNum(); ComboIndex++)
		{
			CurCombo = mWheel->GetCombo(ComboIndex);
			for (size_t StrStart = 0; StrStart < CurCombo.length() - 1; StrStart++)
			{
				std::string SubStr = CurCombo.substr(StrStart, CurCombo.length() - StrStart);
				if (mDict->IsWordValid(SubStr))
				{
					mWordsFound.insert(WheelWord(SubStr, StrStart));
				}
			}
		}
	}
}

void WordFinder::FindWords_MultiThread(uint32_t ThreadNum)
{
	mWordsFound.clear();
	size_t ComboGroupNum = mWheel->GetComboGroupNum();
	std::vector<std::thread> Threads(ThreadNum);
	for (uint32_t i = 0; i < ThreadNum; i++)
	{
		Threads[i] = std::thread(&WordFinder::FindWords_ThreadFunc, this, i, ThreadNum, ComboGroupNum);
	}

	for (uint32_t i = 0; i < ThreadNum; i++)
	{
		if (Threads[i].joinable())
		{
			Threads[i].join();
		}
	}
}

void WordFinder::FindWords_BFS()
{
	mWordsFound.clear();
	for (size_t StartIndex = 0; StartIndex < mWheel->GetWheelNum() - 1; StartIndex++)
	{
		FindWords_BFS_Func("", StartIndex);
	}
}

void WordFinder::FindWords_BFS_MultiThread(uint32_t ThreadNum)
{
	mLevelCount = 0;
	mWordsFound.clear();
	std::vector<std::thread> Threads(ThreadNum);
	for (uint32_t i = 0; i < ThreadNum; i++)
	{
		Threads[i] = std::thread(&WordFinder::FindWords_BFS_ThreadFunc, this);
	}

	for (uint32_t i = 0; i < ThreadNum; i++)
	{
		if (Threads[i].joinable())
		{
			Threads[i].join();
		}
	}
}

void WordFinder::PrintWords()
{
	for (const WheelWord& WordStruct : mWordsFound)
	{
		std::cout << WordStruct.Word << "\n";
	}
	std::cout << "Found " << mWordsFound.size() << " words" << std::endl;
}

void WordFinder::FindWords_BFS_Func(std::string StrBefore, uint32_t Level)
{
	if (Level >= mWheel->GetWheelNum())
		return;

	std::string CurWheel = mWheel->GetWheel(Level);
	for (char Letter : CurWheel)
	{
		std::string CurStr = StrBefore + Letter;
		if (mDict->IsWordValid(CurStr))
		{
			//mWordsFound.insert(WheelWord(CurStr, Level - StrBefore.length()));
			InsertFoundWord(WheelWord(CurStr, Level - StrBefore.length()));
		}
		FindWords_BFS_Func(CurStr, Level + 1);
	}
}

void WordFinder::FindWords_BFS_ThreadFunc()
{
	uint32_t CurStartLevel = 0;
	while (GetNextStartLevel(CurStartLevel))
	{
		FindWords_BFS_Func("", CurStartLevel);
	}
}

void WordFinder::FindWords_ThreadFunc(uint32_t ThreadId, uint32_t Strip, size_t ComboGroupNum)
{
	for (size_t ComboGroupIndex = ThreadId; ComboGroupIndex < ComboGroupNum; ComboGroupIndex += Strip)
	{
		size_t ComboGroupStart = ComboGroupIndex * mWheel->GetLetterNum();

		std::string CurCombo = mWheel->GetCombo(ComboGroupStart);
		for (size_t StrStart = 0; StrStart < CurCombo.length() - 1; StrStart++)
		{
			for (size_t StrLength = 2; StrLength <= CurCombo.length() - StrStart; StrLength++)
			{
				std::string SubStr = CurCombo.substr(StrStart, StrLength);
				if (mDict->IsWordValid(SubStr))
				{
					InsertFoundWord(WheelWord(SubStr, StrStart));
				}
			}
		}
		
		for (size_t ComboIndex = ComboGroupStart + 1; ComboIndex < ComboGroupStart + mWheel->GetLetterNum(); ComboIndex++)
		{
			CurCombo = mWheel->GetCombo(ComboIndex);
			for (size_t StrStart = 0; StrStart < CurCombo.length() - 1; StrStart++)
			{
				std::string SubStr = CurCombo.substr(StrStart, CurCombo.length() - StrStart);
				if (mDict->IsWordValid(SubStr))
				{
					InsertFoundWord(WheelWord(SubStr, StrStart));
				}
			}
		}
		
	}
}

bool WordFinder::InsertFoundWord(const WheelWord& Word)
{
	std::lock_guard<std::mutex> FoundLock(*mFoundMtx.get());
	bool InserRes = mWordsFound.insert(Word).second;
	return InserRes;
}

bool WordFinder::GetNextStartLevel(uint32_t& StartLevel)
{
	std::lock_guard<std::mutex> LevelLock(*mLevelMtx.get());
	if(mLevelCount >= mWheel->GetWheelNum() - 1)
		return false;
	StartLevel = mLevelCount;
	mLevelCount++;
	return true;
}
