#include "assert.h"
#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    // Type Size Assertions
    assert(sizeof(float) * 8 == 32);
    assert(sizeof(double) * 8 == 64);

    // Main
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();

    return a.exec();
}
