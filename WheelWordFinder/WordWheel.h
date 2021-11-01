#pragma once
#include <memory>
#include <string>
#include <vector>

class WordWheel
{
public:
	WordWheel(const std::string& WheelFile);
	~WordWheel();

	uint32_t GetLetterNum();
	uint32_t GetWheelNum();
	size_t GetComboNum();
	size_t GetComboGroupNum();

	std::string& operator[](size_t Index);
	std::string& GetWheel(size_t Index);

	std::string GetCombo(size_t Index);

protected:
	uint32_t mWheelNum = 0;
	uint32_t mLetterNum = 0;
	std::vector<std::string> mWheelData;
};

typedef std::shared_ptr<WordWheel> WordWheelPtr;