#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <atomic>
#include <QList>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QThread>
#include "../include/nlohmann/json.hpp"
#include "shared/constants.h"
#include "shared/structures.h"
#include "shared/configuration.h"
#include "engine/engine.h"
#include "ui/engineworker.h"
#include "ui/abstractwidget.h"
#include "ui/levelmeterwidget.h"
#include "ui/filterwidget.h"
#include "ui/compwidget.h"
#include "ui/kickwidget.h"
#include "ui/sampleplayerwidget.h"
#include "ui/djdeckwidget.h"


namespace Ui {
    class MainWindow;
}


//Configuration load_configuration();


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(const Configuration *configuration, EngineWorker *engineWorker, QWidget *parent = 0);
    ~MainWindow();
    static UiStatus callbackGetStatus(void * thisPtr);
    static void callbackSetStatus(void * thisPtr, EngineStatus status);
    UiStatus getStatus();
    void setEngineStatus(EngineStatus status);

private:
    void _setupUi();
    void _loadPatch();
    Ui::MainWindow *ui;
    const Configuration *_configuration;
    EngineWorker *_engineWorker = nullptr;
    QTimer *_timerRefresh = nullptr;
    bool _started = false;
    EngineStatus _engineStatus;
    UiStatus _uiStatus;
    QList<AbstractWidget*> _modules;
    QList<QLabel*> _nameLabels;
    QList<QLabel*> _valueLabels;
    QList<QSlider*> _sliders;
    float _previousValues[MIDI_ENCODER_COUNT] = {0, 0, 0, 0, 0};
    std::atomic<bool> _statusLocked{false};

private slots:
    void _refresh();
    void _selectedChanged();
    void _stop();
};

#endif // MAINWINDOW_H
