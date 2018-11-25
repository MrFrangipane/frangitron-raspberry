#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::_refresh()
{
    AudioMidiStatus status = _audioWorker->status();

    if( status.input.compressor == nullptr) return;

    // READ
    ui->inputTrack->update_(status.input);

    ui->rmsOutL->setValue(fmax(0, ui->rmsOutL->maximum() + status.input.levelIn->rmsL));
    ui->rmsOutR->setValue(fmax(0, ui->rmsOutR->maximum() + status.input.levelIn->rmsR));
    ui->levelMeterInput->update_((void*)status.input.levelIn);

    // WRITE

    _audioWorker->update(status);
}
