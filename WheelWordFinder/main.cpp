#include <iostream>
#include <chrono>
//#include <fstream>
//#include <set>


#include "WordWheel.h"
#include "WordDictionary.h"
#include "WordFinder.h"


int main(int argc, char* argv[])
{
	/*
	WordWheelPtr TheWheel = nullptr;
	try
	{
		TheWheel.reset(new WordWheel("./Input/Wheels_easy.txt"));
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	if (!TheWheel) return -1;

	std::cout << TheWheel->GetWheelNum() << " " << TheWheel->GetLetterNum() << std::endl;
	for (uint32_t i = 0; i < TheWheel->GetWheelNum(); i++)
	{
		std::cout << TheWheel->GetWheel(i) << std::endl;
	}
	size_t ComboNum = TheWheel->GetComboNum();
	for (size_t i = 0; i < ComboNum; i++)
	{
		std::cout << TheWheel->GetCombo(i) << " ";
	}
	std::cout << std::endl;
	*/
	/*
	WordDictionaryPtr TheDict = nullptr;
	TheDict.reset(new WordDictionary("./Input/dictionary_easy.txt"));
	TheDict->PrintDictionary();
	std::cout << TheDict->IsWordValid("NO") << std::endl;
	*/

	if (argc < 3)
	{
		std::cout << "Not enough input parameters." << std::endl;
		return -1;
	}
	std::string WheelFile = argv[1];
	std::string DictFile = argv[2];

	try
	{
		WordWheelPtr TheWheel = nullptr;
		WordDictionaryPtr TheDict = nullptr;
		WordFinderPtr TheFinder = nullptr;
		TheWheel.reset(new WordWheel(WheelFile));
		TheDict.reset(new WordDictionary(DictFile));
		//TheDict->PrintDictionary();

		std::cout << "How many threads do you want to use? (Please enter a number in range 1-" << TheWheel->GetWheelNum() - 1 << ")" << std::endl;
		int ThreadNum = 1;
		std::cin >> ThreadNum;
		ThreadNum = ThreadNum < 1 ? 1 : ThreadNum;
		ThreadNum = ThreadNum > TheWheel->GetWheelNum() - 1 ? TheWheel->GetWheelNum() - 1 : ThreadNum;
		std::cout << "Use " << ThreadNum << " threads." << std::endl;
		std::cout << "-------------------------------" << std::endl;


		TheFinder.reset(new WordFinder(TheWheel, TheDict));
		auto StartTp = std::chrono::system_clock::now();
		if (ThreadNum > 1)
		{
			TheFinder->FindWords_BFS_MultiThread(ThreadNum);
		}
		else
		{
			TheFinder->FindWords_BFS();
		}
		//TheFinder->FindWords();
		//TheFinder->FindWords_MultiThread(8);
		auto EndTp = std::chrono::system_clock::now();
		double FindTime = (double)std::chrono::duration_cast<std::chrono::milliseconds>(EndTp - StartTp).count();
		TheFinder->PrintWords();
		std::cout <<  "Finish time: " << (double)std::chrono::duration_cast<std::chrono::milliseconds>(EndTp - StartTp).count() << "ms" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	

	system("Pause");


	return 0;
}