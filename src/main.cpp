#include "mainwindow.h"
#include "assert.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    assert(sizeof(float) * 8 == 32);

    QApplication a(argc, argv);
    MainWindow w;
    if( argc == 2 ) w.buffer_size = atoi(argv[1]);
    w.show();

    return a.exec();
}
