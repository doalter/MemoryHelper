#pragma once
#include <QString>
#include <chrono>
#include <memory>
#include <array>

using std::chrono::system_clock;
using std::array;

typedef std::chrono::time_point<system_clock> Date;

struct MemoryData
{
	Date preDate;  //the last date when reviewed the memory
	Date nextDate;  //the next date to review the memory
	Date afterNextDate;  //date after next, a second chance to review when you missed the nextDate
	unsigned int reviewCnt;  //number of reviews
	QString keyWords;    //key words for review
	QString sentence;   //sentence to review
};

struct RawMemory
{
	QString sentence;
	QString keywords;
};

typedef std::shared_ptr<MemoryData> MemoryPtr;


const array<int, 8> incDate = { 0, 1, 2, 4, 7, 15, 30, 60 }; //used for calculating next date to review.