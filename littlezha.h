#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_littlezha.h"
#include "newtaskwidget.h"
#include "forgettingAlgorithm.h"
#include "newlist.h"
#include "tasklist.h"

class LittleZha : public QMainWindow
{
    Q_OBJECT

public:
    LittleZha(QWidget *parent = Q_NULLPTR);
    void closeEvent(QCloseEvent* event);

private:
    Ui::LittleZhaClass ui;
    std::shared_ptr<NewTaskWidget> newTaskWidget;
    std::shared_ptr<NewList> newList;
    std::shared_ptr<TaskList> taskList;
    ForgettingAlgorithm forgettingAlgorithm;

private:
    void initialize();
    void showMemorySize();
    void readData();
    void saveData();

private slots:
    void onPbAddTask();
    void addNewMemory(RawMemory);
    void onPbNew();
    void onPbOneDay();
    void onPbTwoDay();
    void onPbFourDay();
    void onPbSevenDay();
    void onPbFifteenDay();
    void onPbThirtyDay();
    void onPbSixtyDay();
    void onPbTodayTask();
    void refreshData();
};
