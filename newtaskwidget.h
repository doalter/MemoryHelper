#pragma once

#include <QWidget>
#include "ui_newtaskwidget.h"
#include "dataStruct.h"

class NewTaskWidget : public QWidget
{
	Q_OBJECT

public:
	NewTaskWidget(QWidget *parent = Q_NULLPTR);
	~NewTaskWidget();

private:
	Ui::NewTaskWidget ui;

private slots:
	void onSaveMemory();

signals:
	void saveMemorySignal(RawMemory rawMem); //send memory to Main window to save.
};
