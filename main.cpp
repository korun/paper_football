#include "mainwindow.h"
#include <QApplication>
#include <QDebug>       // TODO remove in production

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
