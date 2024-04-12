#include "MyMotorTool.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyMotorTool w;
    w.show();
    return a.exec();
}
