#include "littlezha.h"
#include "vector"
#include "time.h"


LittleZha::LittleZha(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initialize();

    connect(ui.pbAddTask, SIGNAL(clicked()), this, SLOT(onPbAddTask()));
    connect(ui.pbCheckNew, SIGNAL(clicked()), this, SLOT(onPbNew()));
    connect(ui.pbCheckOneDay, SIGNAL(clicked()), this, SLOT(onPbOneDay()));
    connect(ui.pbCheckTwoDay, SIGNAL(clicked()), this, SLOT(onPbTwoDay()));
    connect(ui.pbCheckFourDay, SIGNAL(clicked()), this, SLOT(onPbFourDay()));
    connect(ui.pbCheckSevenDay, SIGNAL(clicked()), this, SLOT(onPbSevenDay()));
    connect(ui.pbCheckFifteenDay, SIGNAL(clicked()), this, SLOT(onPbFifteenDay()));
    connect(ui.pbCheckThirtyDay, SIGNAL(clicked()), this, SLOT(onPbThirtyDay()));
    connect(ui.pbCheckSixtyDay, SIGNAL(clicked()), this, SLOT(onPbSixtyDay()));
    connect(ui.pbCheckTodayTask, SIGNAL(clicked()), this, SLOT(onPbTodayTask()));
}

void LittleZha::initialize()
{
    readData();
    forgettingAlgorithm.taskToReview();
    showMemorySize();
}

void LittleZha::showMemorySize()
{
    ui.lcdNew->display(forgettingAlgorithm.getListSize("newMemory"));
    ui.lcdOneDay->display(forgettingAlgorithm.getListSize("oneDayMemory"));
    ui.lcdTwoDay->display(forgettingAlgorithm.getListSize("twoDayMemory"));
    ui.lcdFourDay->display(forgettingAlgorithm.getListSize("fourDayMemory"));
    ui.lcdSevenDay->display(forgettingAlgorithm.getListSize("sevenDayMemory"));
    ui.lcdFifteenDay->display(forgettingAlgorithm.getListSize("fifteenDayMemory"));
    ui.lcdThirtyDay->display(forgettingAlgorithm.getListSize("thirtyDayMemory"));
    ui.lcdSixtyDay->display(forgettingAlgorithm.getListSize("sixtyDayMemory"));
    ui.lcdToday->display(forgettingAlgorithm.getTaskSize());
}

void LittleZha::onPbAddTask()
{
    newTaskWidget = std::make_shared<NewTaskWidget>();
    connect(newTaskWidget.get(), SIGNAL(saveMemorySignal(RawMemory)), this, SLOT(addNewMemory(RawMemory)));
    newTaskWidget->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newTaskWidget->show();
}

void LittleZha::addNewMemory(RawMemory rawMem)
{
    forgettingAlgorithm.addNewMemory(rawMem);
    ui.lcdNew->display(forgettingAlgorithm.getListSize("newMemory"));
}

void LittleZha::onPbNew()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("newMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

//reuse newList for other memory(one day, two day, four day ...)
void LittleZha::onPbOneDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("oneDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbTwoDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("twoDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbFourDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("fourDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbSevenDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("sevenDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbFifteenDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("fifteenDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbThirtyDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("thirtyDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}

void LittleZha::onPbSixtyDay()
{
    newList = std::make_shared<NewList>(forgettingAlgorithm.getList("sixtyDayMemory"));
    newList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    newList->show();
}


void LittleZha::onPbTodayTask()
{
    taskList = std::make_shared<TaskList>(forgettingAlgorithm.getTaskList());
    connect(taskList.get(), SIGNAL(refreshDataSignal()), this, SLOT(refreshData()));
    taskList->setWindowModality(Qt::ApplicationModal); //The window is modal to the application and blocks input to all windows. 
    taskList->show();
}

void LittleZha::refreshData()
{
    forgettingAlgorithm.putTaskIntoList(); //some task has been reviewed, then put them into the correct list.
    forgettingAlgorithm.taskToReview();
    showMemorySize();
}

void LittleZha::readData()
{
    std::vector<string> memName = { "newMemory", "oneDayMemory", "twoDayMemory", "fourDayMemory",
                              "sevenDayMemory", "fifteenDayMemory", "thirtyDayMemory", "sixtyDayMemory"};
    for (string name : memName)
    {
        string filename = name + ".dat";
        QFile file(QString::fromStdString(filename));

        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        QDataStream in(&file);
        int listSize = 0;
        in >> listSize;
        while (listSize != 0)
        {
            time_t preDate;
            time_t nextDate;
            time_t afterNextDate;
            MemoryData data;
            in >> preDate >> nextDate >> afterNextDate;
            in >> data.reviewCnt >> data.keyWords >> data.sentence;
            data.preDate = system_clock::from_time_t(preDate);
            data.nextDate = system_clock::from_time_t(nextDate);
            data.afterNextDate = system_clock::from_time_t(afterNextDate);
            forgettingAlgorithm.setMemory(name, data);
            listSize--;
        }
        file.close();
    }
}

void LittleZha::saveData()
{
    std::vector<string> memName = { "newMemory", "oneDayMemory", "twoDayMemory", "fourDayMemory",
                              "sevenDayMemory", "fifteenDayMemory", "thirtyDayMemory", "sixtyDayMemory" };
    for (string name : memName)
    {
        string filename = name + ".dat";
        QFile file(QString::fromStdString(filename));
        if (filename == "sixtyDayMemory")
        {
            file.open(QIODevice::WriteOnly | QIODevice::Append);
        }
        else
        { 
            file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        }
        QDataStream out(&file);
        auto memList = forgettingAlgorithm.getList(name);
        out << forgettingAlgorithm.getListSize(name); // The first data to save is the size of each memList.
        for (auto mem : memList)
        {
            time_t preDate = std::chrono::system_clock::to_time_t(mem->preDate);
            time_t nextDate = std::chrono::system_clock::to_time_t(mem->nextDate);
            time_t afterNextDate = std::chrono::system_clock::to_time_t(mem->afterNextDate);
            out << preDate << nextDate << afterNextDate;
            out << mem->reviewCnt << mem->keyWords << mem->sentence;
        }
        file.close();
    }
}

void LittleZha::closeEvent(QCloseEvent* event)
{
    forgettingAlgorithm.putTaskIntoList(); //some task has NOT been reviewd this time, put them back.
    saveData();
    event->accept();
}