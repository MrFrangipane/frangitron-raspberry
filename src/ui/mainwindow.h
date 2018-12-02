#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <atomic>
#include <QList>
#include <QWidget>
#include <QThread>
#include <QString>
#include <QTimer>
#include "../include/nlohmann/json.hpp"
#include "audio/engine.h"
#include "ui/engineworker.h"
#include "ui/abstractwidget.h"
#include "ui/levelmeterwidget.h"
#include "ui/filterwidget.h"
#include "ui/compwidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static EngineStatus callbackGetStatus(void * thisPtr);
    static void callbackSetStatus(void * thisPtr, EngineStatus status);
    EngineStatus getEngineStatus() const;
    void setEngineStatus(EngineStatus status);

private:
    Ui::MainWindow *ui;
    QThread* _engineThread = nullptr;
    EngineWorker* _engineWorker = nullptr;
    QTimer* _timerRefresh = nullptr;
    bool _started = false;
    EngineStatus _engineStatus;
    QList<AbstractWidget*> _moduleWidgets;
    void _setupUi();
    std::atomic<bool> _isWritingStatus{false};

private slots:
    void _refresh();
    void _selectedChanged();
};

#endif // MAINWINDOW_H
