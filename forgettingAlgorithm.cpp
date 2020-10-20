#include "forgettingAlgorithm.h"
#include <array>
#include <ctime>
#include <memory>
#include <vector>

using std::make_shared;


ForgettingAlgorithm::ForgettingAlgorithm()
{
	//newMemory: new task
	//oneDayMemory: reviewed once
	//twoDayMemory: reviewed twice
	//sixtyDayMemory: reviewed 60 days later, permanent memory.
	allMemory.insert(std::make_pair(string("newMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("oneDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("twoDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("fourDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("sevenDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("fifteenDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("thirtyDayMemory"), list<MemoryPtr>()));
	allMemory.insert(std::make_pair(string("sixtyDayMemory"), list<MemoryPtr>()));
}

void ForgettingAlgorithm::taskToReview()
{
	refreshAllMemoryList();
}


void ForgettingAlgorithm::refreshAllMemoryList()
{
	for (auto& memoryList : allMemory)
	{
		if (memoryList.first != "sixtyDayMemory") //permanent memory, no need to review
		{
			refreshSigleMemoryList(memoryList.second);
		}		
	}
}

void ForgettingAlgorithm::refreshSigleMemoryList(std::list<MemoryPtr>& memList)
{
	if (memList.empty())
	{
		return;
	}

	//auto currentDate = std::chrono::system_clock::now() + std::chrono::hours(24 * incDate[1]);
	auto currentDate = std::chrono::system_clock::now();
	for (auto iter = memList.begin(); iter != memList.end();)
	{
		auto nextDate = (*iter)->nextDate;
		auto afterNextDate = (*iter)->afterNextDate;
		if (dateBefore(currentDate, nextDate))
		{
			++iter;
		}
		else if (dateEqual(currentDate, nextDate) ||
				dateBefore(currentDate, afterNextDate) || 
				dateEqual(currentDate, afterNextDate))
		{
			//add the memory to the review list and delete it from the memList
			reviewList.push_back(*iter); 
			memList.erase(iter++); //move the iter to the next element first, then erase.
		}
		// long time since last time reviewed, put the memory to newMemory
		else
		{
			RawMemory rawMem;
			rawMem.sentence = (*iter)->sentence;
			rawMem.keywords = (*iter)->keyWords;
			addNewMemory(rawMem);
			memList.erase(iter++);
		}
	}
}

//compare day only, ignore hour, minute and second.
bool ForgettingAlgorithm::dateEqual(const Date& currentDate, const Date& compareDate)
{
	auto cur = system_clock::to_time_t(currentDate);
	auto currentTime = localtime(&cur);
	auto compare = system_clock::to_time_t(compareDate);
	auto compareTime = localtime(&compare);

	return (currentTime->tm_year == compareTime->tm_year
		&& currentTime->tm_yday == compareTime->tm_yday);
}

//compare day only, ignore hour, minute and second.
bool ForgettingAlgorithm::dateBefore(const Date& currentDate, const Date& compareDate)
{
	auto cur = system_clock::to_time_t(currentDate);
	auto currentTime = localtime(&cur);
	//localtime returns pointer which points to a static memory, so need to save the time before calling loacltime again.
	auto curYear = currentTime->tm_year;
	auto curDay = currentTime->tm_yday;

	auto compare = system_clock::to_time_t(compareDate);
	auto compareTime = localtime(&compare);
	auto compareYear = compareTime->tm_year;
	auto compareDay = compareTime->tm_yday;

	if (curYear < compareYear)
	{
		return true;
	}

	if (curYear > compareYear)
	{
		return false;
	}

	return (curDay < compareDay);
}

void ForgettingAlgorithm::addNewMemory(const RawMemory& rawMem)
{
	MemoryPtr memPtr = make_shared<MemoryData>();
	memPtr->sentence = rawMem.sentence;
	memPtr->keyWords = rawMem.keywords;
	memPtr->preDate = std::chrono::system_clock::now();
	memPtr->reviewCnt = 0;
	int next = 1;
	int afterNext = 2;
	//Make sure the date is correct, ignores hour/minute/second, since the date compare algorithm ignores hour, minute and second.
	memPtr->nextDate = memPtr->preDate + std::chrono::hours(24 * incDate[next]);
	memPtr->afterNextDate = memPtr->preDate + std::chrono::hours(24 * incDate[afterNext]);
	allMemory["newMemory"].push_back(memPtr);
}

void ForgettingAlgorithm::setMemory(const string& listName, const MemoryData& mem)
{
	MemoryPtr memPtr = make_shared<MemoryData>(mem);
	allMemory[listName].push_back(memPtr);
}

int ForgettingAlgorithm::getListSize(const string& listName)
{
	return allMemory[listName].size();
}

int ForgettingAlgorithm::getTaskSize()
{
	return reviewList.size();
}

const list<MemoryPtr>& ForgettingAlgorithm::getList(const string& listName)
{
	return allMemory[listName];
}

const list<MemoryPtr>& ForgettingAlgorithm::getTaskList()
{
	return reviewList;
}

//Put task into corresponding list by reviewCnt.
void ForgettingAlgorithm::putTaskIntoList()
{
	std::vector<string> memName = { "newMemory", "oneDayMemory", "twoDayMemory", "fourDayMemory",
							        "sevenDayMemory", "fifteenDayMemory", "thirtyDayMemory", "sixtyDayMemory" };
	while (reviewList.size())
	{
		auto headMem = reviewList.front();
		auto headIter = reviewList.begin();
		allMemory[memName[headMem->reviewCnt]].push_back(headMem);
		reviewList.erase(headIter);
	}
}