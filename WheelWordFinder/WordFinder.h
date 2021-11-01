#pragma once
#include "WordWheel.h"
#include "WordDictionary.h"
#include <set>
#include <mutex>

struct WheelWord
{
	std::string Word;
	uint32_t StartIndex;

	WheelWord(const std::string& InWord, uint32_t InStartIndex)
		: Word(InWord)
		, StartIndex(InStartIndex)
	{};

	friend bool operator<(const WheelWord& x, const WheelWord& y)
	{
		if (x.Word == y.Word)
			return x.StartIndex < y.StartIndex;
		return x.Word < y.Word;
	}
};

class WordFinder
{
public:
	WordFinder(const WordWheelPtr& InWheel, const WordDictionaryPtr& InDict);
	~WordFinder();

	void FindWords();
	void FindWords_MultiThread(uint32_t ThreadNum);

	void FindWords_BFS();
	void FindWords_BFS_MultiThread(uint32_t ThreadNum);

	void PrintWords();

protected:
	void FindWords_BFS_Func(std::string StrBefore, uint32_t Level);
	void FindWords_BFS_ThreadFunc();
	void FindWords_ThreadFunc(uint32_t ThreadId, uint32_t Strip, size_t ComboGroupNum);

	bool InsertFoundWord(const WheelWord& Word);
	bool GetNextStartLevel(uint32_t& StartLevel);

protected:
	WordWheelPtr mWheel;
	WordDictionaryPtr mDict;
	std::set<WheelWord> mWordsFound;
	

private:
	std::unique_ptr<std::mutex> mFoundMtx;
	uint32_t mLevelCount = 0;
	std::unique_ptr<std::mutex> mLevelMtx;
};

typedef std::shared_ptr<WordFinder> WordFinderPtr;