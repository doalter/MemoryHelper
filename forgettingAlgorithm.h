//Copyright Xiaofeng Cheng 2020
//Author:   Xiaofeng Cheng
//Date:     Sep. 14th, 2020
//History:  Sep. 14th, 2020		created.


#pragma once
#include <list>
#include <map>
#include <string>
#include "dataStruct.h"

using std::list;
using std::map;
using std::string;

class ForgettingAlgorithm
{
public:
	ForgettingAlgorithm();
	void taskToReview();
	void addNewMemory(const RawMemory& rawMem);
	int getListSize(const string& listName);
	int getTaskSize();
	const list<MemoryPtr>& getList(const string& listName);
	const list<MemoryPtr>& getTaskList();
	void setMemory(const string& listName, const MemoryData& mem);
	void putTaskIntoList();

private:
	void refreshAllMemoryList();
	void refreshSigleMemoryList(std::list<MemoryPtr>& memList);
	//check if currentDate == compareDate
	bool dateEqual(const Date& currentDate, const Date& compareDate);
	//check if currentDate < compareDate
	bool dateBefore(const Date& currentDate, const Date& compareDate);

private:
	map<string, list<MemoryPtr>> allMemory;
	list<MemoryPtr> reviewList;
};


