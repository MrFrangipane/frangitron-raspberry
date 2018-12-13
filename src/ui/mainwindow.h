#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <atomic>
#include <QList>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QThread>
#include <QString>
#include <QTimer>
#include "../include/nlohmann/json.hpp"
#include "shared/structures.h"
#include "engine/engine.h"
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
    static UiStatus callbackGetStatus(void * thisPtr);
    static void callbackSetStatus(void * thisPtr, EngineStatus status);
    UiStatus getStatus();
    void setEngineStatus(EngineStatus status);

private:
    Ui::MainWindow *ui;
    QThread* _engineThread = nullptr;
    EngineWorker* _engineWorker = nullptr;
    QTimer* _timerRefresh = nullptr;
    bool _started = false;
    EngineStatus _engineStatus;
    UiStatus _uiStatus;
    QList<AbstractWidget*> _modules;
    QList<QLabel*> _nameLabels;
    QList<QLabel*> _valueLabels;
    QList<QSlider*> _sliders;
    float _previousValues[5] = {0, 0, 0, 0, 0};
    void _setupUi();
    std::atomic<bool> _statusLocked{false};

private slots:
    void _refresh();
    void _selectedChanged();
};

#endif // MAINWINDOW_H
