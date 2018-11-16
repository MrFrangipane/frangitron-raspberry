#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->exit, SIGNAL(clicked(bool)), this, SLOT(close()));

    _audioThread = new QThread();
    _audioThread->setObjectName("AudioMidi");
    _audioWorker = new AudioWorker();
    _audioWorker->moveToThread(_audioThread);
    connect(_audioThread, SIGNAL(started()), _audioWorker, SLOT(process()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(quit()));
    connect(_audioWorker, SIGNAL(finished()), _audioWorker, SLOT(deleteLater()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(deleteLater()));
    _audioThread->start();
    _audioThread->setPriority(QThread::TimeCriticalPriority);

    _timerRefresh = new QTimer();
    connect(_timerRefresh, SIGNAL(timeout()), this, SLOT(_refresh()));
    _timerRefresh->start(1000 / 30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_refresh()
{
    Status status = _audioWorker->status();

    ui->compLevel->setValue((1.0 - status.compIn) * 100);

    ui->rmsInL->setValue(fmax(0, ui->rmsInL->maximum() + status.rmsInL));
    ui->rmsInR->setValue(fmax(0, ui->rmsInR->maximum() + status.rmsInR));

    ui->rmsOutL->setValue(fmax(0, ui->rmsOutL->maximum() + status.rmsOutL));
    ui->rmsOutR->setValue(fmax(0, ui->rmsOutR->maximum() + status.rmsOutR));
}
