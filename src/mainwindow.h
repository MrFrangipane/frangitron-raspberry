#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QThread>
#include "audioworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int buffer_size;

private:
    Ui::MainWindow *ui;
    QThread* _audioThread;
    AudioWorker* _audioWorker;
};

#endif // MAINWINDOW_H
