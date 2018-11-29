#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QWidget>
#include <QThread>
#include <QString>
#include <QTimer>
#include "audiomidi.h"
#include "audioworker.h"
#include "../include/nlohmann/json.hpp"

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
    QThread* _audioThread = nullptr;
    AudioWorker* _audioWorker = nullptr;
    QTimer* _timerRefresh = nullptr;
    bool _started = false;
    AudioMidiStatus _status;
    void setupUi();

private slots:
    void _refresh();
    void _selectedChanged();
};

#endif // MAINWINDOW_H
