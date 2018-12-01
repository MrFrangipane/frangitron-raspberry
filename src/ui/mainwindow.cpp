#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    setupUi();

    _audioThread = new QThread();
    _audioThread->setObjectName("AudioMidi");
    _audioWorker = new EngineWorker();
    _audioWorker->moveToThread(_audioThread);
    connect(_audioThread, SIGNAL(started()), _audioWorker, SLOT(process()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(quit()));
    connect(_audioWorker, SIGNAL(finished()), _audioWorker, SLOT(deleteLater()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(deleteLater()));
    _audioThread->start();

    QThread::currentThread()->setPriority(QThread::LowPriority);

    _timerRefresh = new QTimer();
    connect(_timerRefresh, SIGNAL(timeout()), this, SLOT(_refresh()));
    _timerRefresh->start(1000 / UI_FRAMERATE);

    connect(ui->exit, SIGNAL(clicked(bool)), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    ui->setupUi(this);

    _moduleWidgets << new LevelMeterWidget();
    _moduleWidgets <<  new FilterWidget();
    _moduleWidgets << new CompWidget();
    _moduleWidgets << new LevelMeterWidget();

    connect(_moduleWidgets[0], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(_moduleWidgets[1], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(_moduleWidgets[2], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(_moduleWidgets[3], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));

    _moduleWidgets[0]->setProperty("displayName", "IN");
    _moduleWidgets[1]->setProperty("displayName", "IN FILTER");
    _moduleWidgets[2]->setProperty("displayName", "IN COMP");
    _moduleWidgets[3]->setProperty("displayName", "MASTER");

    ui->layoutPatch->addWidget(_moduleWidgets[0], 0, 0, 2, 1);
    ui->layoutPatch->addWidget(_moduleWidgets[1], 0, 1);
    ui->layoutPatch->addWidget(_moduleWidgets[2], 1, 1);
    ui->layoutPatch->addWidget(_moduleWidgets[3], 0, 3, 2, 1);
}

void MainWindow::_selectedChanged()
{
    for( AbstractWidget* moduleWidget : _moduleWidgets ) {
        if( moduleWidget != sender() ) {
            moduleWidget->desselect();
        }
    }
}

void MainWindow::_refresh()
{
    _status = _audioWorker->status();
    if( _status.moduleStatuses.empty() ) return;

    // STATUS -> UI
    int i = 0;
    for( AbstractWidget* moduleWidget : _moduleWidgets ) {
        moduleWidget->update_(_status.moduleStatuses[i]);
        i++;
    }

    // UI -> STATUS
    _status.moduleStatuses[1]["cutoff"] = ((float)ui->sliderEnc1->value() / 500.0) - 1.0;
    _audioWorker->update(_status);
}
