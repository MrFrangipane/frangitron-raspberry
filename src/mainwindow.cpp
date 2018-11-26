#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    setupUi();

    _audioThread = new QThread();
    _audioThread->setObjectName("AudioMidi");
    _audioWorker = new AudioWorker();
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

    connect(ui->levelMeterInput, SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(ui->filterInput, SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(ui->compInput, SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    connect(ui->levelMeterOutput, SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));

}

void MainWindow::_selectedChanged()
{
    if( ui->levelMeterInput != sender() ) ui->levelMeterInput->desselect();
    if( ui->filterInput != sender() ) ui->filterInput->desselect();
    if( ui->compInput != sender() ) ui->compInput->desselect();
    if( ui->levelMeterOutput != sender() ) ui->levelMeterOutput->desselect();
}

void MainWindow::_refresh()
{
    _status = _audioWorker->status();

    ui->levelMeterInput->update_((void*)&_status.meterInput);
    ui->filterInput->update_((void*)&_status.filterInput);

    ui->levelMeterOutput->update_((void*)&_status.meterOutput);
}
