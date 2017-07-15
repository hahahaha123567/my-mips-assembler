#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyWidget w;
    w.showMaximized();
    w.show();

    return a.exec();
}
