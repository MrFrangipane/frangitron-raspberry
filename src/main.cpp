#include "assert.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Type Size Assertions
    assert(sizeof(float) * 8 == 32);
    assert(sizeof(double) * 8 == 64);

    // Main
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
