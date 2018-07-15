#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyWidget * w = new MyWidget;
    w->showMaximized();

    return a.exec();
}
