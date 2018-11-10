#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QThread>
#include <QString>
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
    int buffer_size = 0;

private:
    Ui::MainWindow *ui;
    QThread* _audioThread;
    AudioWorker* _audioWorker;
};

#endif // MAINWINDOW_H
