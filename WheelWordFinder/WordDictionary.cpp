#include "WordDictionary.h"
#include <fstream>

WordDictionary::WordDictionary(const std::string& DictionaryFile)
{
	std::ifstream InDict(DictionaryFile);
	if (!InDict.is_open())
	{
		throw(std::exception("DictionaryFile not found."));
		return;
	}

	while (!InDict.eof())
	{
		std::string WordStr;
		InDict >> WordStr;
		if (WordStr.length() > 1)
			mDictData.insert(GetUpperCaseWord(WordStr));
	}

	InDict.close();
}

WordDictionary::~WordDictionary()
{
}

bool WordDictionary::IsWordValid(const std::string& Word)
{
	return mDictData.find(Word) != mDictData.end();
}

uint32_t WordDictionary::GetSize()
{
	return mDictData.size();
}

void WordDictionary::PrintDictionary()
{
	for ( const std::string& Word : mDictData)
	{
		printf("%s ", Word.c_str());
	}
	printf("\n");
}

std::string WordDictionary::GetUpperCaseWord(const std::string& InWord)
{
	std::string UpperCaseWord;
	UpperCaseWord.resize(InWord.length());
	for (size_t i = 0; i < InWord.length(); i++)
	{
		char Letter = InWord[i];
		if (Letter >= 97 && Letter <= 122)
		{
			Letter -= 32;
		}
		UpperCaseWord[i] = Letter;
	}

	return UpperCaseWord;
}
