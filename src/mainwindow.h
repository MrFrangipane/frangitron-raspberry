#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QThread>
#include <QString>
#include <QTimer>
#include "audiomidi.h"
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

private:
    Ui::MainWindow *ui;
    QThread* _audioThread;
    AudioWorker* _audioWorker;
    QTimer* _timerRefresh;

private slots:
    void _refresh();
};

#endif // MAINWINDOW_H
