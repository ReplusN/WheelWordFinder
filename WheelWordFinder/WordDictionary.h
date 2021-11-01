#pragma once
#include <memory>
#include <string>
#include <unordered_set>

class WordDictionary
{
public:
	WordDictionary(const std::string& DictionaryFile);
	~WordDictionary();

	bool IsWordValid(const std::string& Word);
	
	uint32_t GetSize();
	void PrintDictionary();


protected:
	std::string GetUpperCaseWord(const std::string& InWord);

protected:
	std::unordered_set<std::string> mDictData;
};

typedef std::shared_ptr<WordDictionary> WordDictionaryPtr;