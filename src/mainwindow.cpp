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
    ui->inputTrack->update_(status.input);

    ui->rmsOutL->setValue(fmax(0, ui->rmsOutL->maximum() + status.input.levelOutL));
    ui->rmsOutR->setValue(fmax(0, ui->rmsOutR->maximum() + status.input.levelOutR));

    ui->labelCompTreshold->setText(QString::number(status.input.compressor.threshold));
    ui->labelCompAttack->setText(QString::number(status.input.compressor.attack));
    ui->labelCompRelease->setText(QString::number(status.input.compressor.release));
    ui->labelCompRatio->setText(QString::number(status.input.compressor.ratio));

    // WRITE
    status.input.compressor.threshold = ui->sliderCompThreshold->value();
    status.input.compressor.attack = ui->sliderCompAttack->value() / 1000.0;
    status.input.compressor.release = ui->sliderCompRelease->value() / 1000.0;
    if( ui->checkComp->isChecked() ) status.input.compressor.ratio = (float)ui->sliderCompRatio->value() / 100.0;
    else status.input.compressor.ratio = 1.0;

    _audioWorker->update(status);
}
