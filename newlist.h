#pragma once

#include <QWidget>
#include "ui_newlist.h"
#include "dataStruct.h"
#include <list>


class NewList : public QWidget
{
	Q_OBJECT

public:
	NewList(QWidget *parent = Q_NULLPTR);
	NewList(const std::list<MemoryPtr>& lst, QWidget* parent = Q_NULLPTR);
	~NewList();
	void showNewMemory();

private:
	Ui::NewList ui;
	std::list<MemoryPtr> memList;

private:
	void initialize();
	
};
