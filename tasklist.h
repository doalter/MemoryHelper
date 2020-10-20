#pragma once

#include <QWidget>
#include "ui_tasklist.h"
#include "dataStruct.h"
#include <QCloseEvent>

class TaskList : public QWidget
{
	Q_OBJECT

public:
	TaskList(QWidget *parent = Q_NULLPTR);
	TaskList(const std::list<MemoryPtr>& lst, QWidget* parent = Q_NULLPTR);
	~TaskList();
	void showTask();
	void closeEvent(QCloseEvent* event);  //Update the memory data when close the window.

private:
	void initialize();
	void initialzeLCD();
	void onContentChanged(int row, int column);
	void reviewed(MemoryPtr mem);

private slots:
	void onPbTaskStatus();
	void onCbHideSentence(int state);

signals:
	void refreshDataSignal();
	
private:
	Ui::TaskList ui;
	std::list<MemoryPtr> memList;
};
