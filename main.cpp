#include "littlezha.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LittleZha w;
    w.show();
    return a.exec();
}
