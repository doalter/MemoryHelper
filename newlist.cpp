#include "newlist.h"
#include <QTextEdit>
#include <QScrollBar> 

NewList::NewList(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NewList::NewList(const std::list<MemoryPtr>& lst, QWidget* parent)
	: memList(lst), QWidget(parent)
{
	ui.setupUi(this);
	initialize();
}

NewList::~NewList()
{
}

void NewList::initialize()
{
	auto size = memList.size();
	int rowNum = size > 10 ? size : 10;
	ui.newMemoryTable->setRowCount(rowNum);
	ui.newMemoryTable->setColumnCount(2);
	QStringList header;
	header << "Memory" << "Keywords";
	ui.newMemoryTable->setHorizontalHeaderLabels(header);
	ui.newMemoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.newMemoryTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	showNewMemory();
}

void NewList::showNewMemory()
{
	int row = 0;
	for (auto memory : memList)
	{
		//Note: There is no memory leak.
		//The pointer will be released by their parent object(QTableWidget),when the QTableWidget is deleted. 
		auto senEdit = new QTextEdit();
		auto keyEdit = new QTextEdit();
		senEdit->setText(memory->sentence);
		//senEdit->resize(senEdit->document()->size().width(), senEdit->document()->size().width() + 10);
		keyEdit->setText(memory->keyWords);
		ui.newMemoryTable->setCellWidget(row, 0, senEdit);
		ui.newMemoryTable->setCellWidget(row, 1, keyEdit);
		row++;
	}
}