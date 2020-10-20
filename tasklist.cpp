#include "tasklist.h"
#include <QTextEdit>
#include <QPushButton>
#include <iostream>

TaskList::TaskList(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TaskList::TaskList(const std::list<MemoryPtr>& lst, QWidget* parent)
	: memList(lst), QWidget(parent)
{
	ui.setupUi(this);
	initialize();
}

TaskList::~TaskList()
{
}

void TaskList::initialize()
{	
	connect(ui.cbHideSentence, SIGNAL(stateChanged(int)), this, SLOT(onCbHideSentence(int)));
	initialzeLCD();
	showTask();
}

void TaskList::showTask()
{
	auto size = memList.size();
	int rowNum = size > 10 ? size : 10;
	ui.taskTable->setRowCount(rowNum);
	ui.taskTable->setColumnCount(3);
	QStringList header;
	header << "Memory" << "Keywords" << "Status";
	ui.taskTable->setHorizontalHeaderLabels(header);
	ui.taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.taskTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	
	int row = 0;
	for (auto memory : memList)
	{
		//Note: There is no memory leak.
		//The pointer will be released by their parent object(QTableWidget), when the QTableWidget is deleted. 
		auto senEdit = new QTextEdit();
		auto keyEdit = new QTextEdit();
		senEdit->setText("******");
		keyEdit->setText(memory->keyWords);
		ui.taskTable->setCellWidget(row, 0, senEdit);
		ui.taskTable->setCellWidget(row, 1, keyEdit);
		connect(senEdit, &QTextEdit::textChanged, [row, this]() {onContentChanged(row, 0);});
		connect(keyEdit, &QTextEdit::textChanged, [row, this]() {onContentChanged(row, 1);});

		auto doneButton = new QPushButton("UNFINISHED");
		doneButton->setStyleSheet("QPushButton{background: rgb(221, 75, 39); color: white; font-family: 'Arial Black'; font-size: 32px; border-radius: 10px;}"
								  "QPushButton:hover{background: rgb(255, 186, 129);}"
								  "QPushButton:pressed{background: rgb(202, 129, 0);}");
		connect(doneButton, SIGNAL(clicked()), this, SLOT(onPbTaskStatus()));
		ui.taskTable->setCellWidget(row, 2, doneButton);
		row++;
	}
}

void TaskList::onPbTaskStatus()
{
	int doneCnt = ui.lcdDoneNumber->intValue();
	int undoneCnt = ui.lcdUndoneNumber->intValue();
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	if (button->text() == "UNFINISHED")
	{
		button->setText("DONE");
		button->setStyleSheet("QPushButton{background: rgb(00, 172, 237); color: white; font-family: 'Arial Black'; font-size: 32px; border-radius: 10px;}"
							  "QPushButton:hover{background: rgb(91, 169, 170);}"
							  "QPushButton:pressed{background: rgb(14, 134, 170);}"); //change the color to green.
		ui.lcdUndoneNumber->display(undoneCnt-1);
		ui.lcdDoneNumber->display(doneCnt+1);
	}
	else
	{
		button->setText("UNFINISHED");
		button->setStyleSheet("QPushButton{background: rgb(221, 75, 39); color: white; font-family: 'Arial Black'; font-size: 32px; border-radius: 10px;}"
							  "QPushButton:hover{background: rgb(255, 186, 129);}"
							  "QPushButton:pressed{background: rgb(202, 0, 0);}"); //change the color to red.
		ui.lcdUndoneNumber->display(undoneCnt+1);
		ui.lcdDoneNumber->display(doneCnt-1);
	}
}

void TaskList::initialzeLCD()
{	
	ui.lcdTotalNumber->setPalette(Qt::blue);
	ui.lcdTotalNumber->display(static_cast<int>(memList.size()));
	
	ui.lcdUndoneNumber->setPalette(Qt::red);
	ui.lcdUndoneNumber->display(static_cast<int>(memList.size()));
	
	ui.lcdDoneNumber->setPalette(Qt::green);
	ui.lcdDoneNumber->display(0);
}

void TaskList::closeEvent(QCloseEvent* event)
{
	int row = 0;
	for (auto mem : memList)
	{
		auto doneButton = qobject_cast<QPushButton*>(ui.taskTable->cellWidget(row, 2));
		if (doneButton->text() == "DONE")
		{
			reviewed(mem);
		}
		row++;
	}
	emit refreshDataSignal();  //send signal to main window to refresh the data.
	event->accept();
}

void TaskList::onContentChanged(int row, int column)
{
	auto contentEdit = qobject_cast<QTextEdit*>(ui.taskTable->cellWidget(row, column));
	auto iter = memList.begin();
	for (int i = 0; i != row; i++)
	{
		iter++;
	}

	if (column == 0)
	{
		if (contentEdit->toPlainText() == "******")
		{
			return;
		}
		(*iter)->sentence = contentEdit->toPlainText(); //update the sentence.
	}
	else
	{
		(*iter)->keyWords = contentEdit->toPlainText(); //update the key.
	}
}

void TaskList::reviewed(MemoryPtr mem)
{
	mem->preDate = std::chrono::system_clock::now();
	mem->reviewCnt++;
	int next = (mem->reviewCnt) + 1;
	int afterNext = next + 1;
	//Make sure the date is correct, ignores hour/minute/second, since the date compare algorithm ignores hour, minute and second.
	mem->nextDate = mem->preDate + std::chrono::hours(24 * incDate[next]);
	mem->afterNextDate = mem->preDate + std::chrono::hours(24 * incDate[afterNext]);
}

void TaskList::onCbHideSentence(int state)
{
	if (state == Qt::Checked)
	{
		auto size = memList.size();
		for (int row = 0; row != size; ++row)
		{
			auto senEdit = qobject_cast<QTextEdit*>(ui.taskTable->cellWidget(row, 0));
			senEdit->setText("******");
		}
	}
	else
	{
		int row = 0;
		for (auto memory : memList)
		{
			auto senEdit = qobject_cast<QTextEdit*>(ui.taskTable->cellWidget(row, 0));
			senEdit->setText(memory->sentence);
			row++;
		}		
	}
}