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
    _timerRefresh->start(1000 / 30);

    connect(ui->exit, SIGNAL(clicked(bool)), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_refresh()
{
    AudioMidiStatus status = _audioWorker->status();

    // READ
    ui->compLevel->setValue((1.0 - status.input.compressor.level) * 100);
    ui->compGate->setChecked(status.input.compressor.gate);

    ui->rmsInL->setValue(fmax(0, ui->rmsInL->maximum() + status.input.levelInL));
    ui->rmsInR->setValue(fmax(0, ui->rmsInR->maximum() + status.input.levelInR));

    ui->rmsOutL->setValue(fmax(0, ui->rmsOutL->maximum() + status.input.levelOutL));
    ui->rmsOutR->setValue(fmax(0, ui->rmsOutR->maximum() + status.input.levelOutR));

    ui->labelCompTreshold->setText(QString::number(status.input.compressor.threshold));
    ui->labelCompAttack->setText(QString::number(status.input.compressor.attack));
    ui->labelCompRelease->setText(QString::number(status.input.compressor.release));

    // WRITE
    status.input.compressor.threshold = ui->sliderCompThreshold->value();
    status.input.compressor.attack = ui->sliderCompAttack->value() / 1000.0;
    status.input.compressor.release = ui->sliderCompRelease->value() / 1000.0;

    _audioWorker->update(status);
}
