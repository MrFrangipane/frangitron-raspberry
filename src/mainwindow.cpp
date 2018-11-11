#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->exit, SIGNAL(clicked(bool)), this, SLOT(close()));
    ui->label->setText(QString::number(buffer_size));

    _audioThread = new QThread();
    _audioThread->setObjectName("AudioMidi");
    _audioWorker = new AudioWorker(buffer_size);
    _audioWorker->moveToThread(_audioThread);
    connect(_audioThread, SIGNAL(started()), _audioWorker, SLOT(process()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(quit()));
    connect(_audioWorker, SIGNAL(finished()), _audioWorker, SLOT(deleteLater()));
    connect(_audioWorker, SIGNAL(finished()), _audioThread, SLOT(deleteLater()));
    _audioThread->start();
    //_audioThread->setPriority(QThread::TimeCriticalPriority);
}

MainWindow::~MainWindow()
{
    delete ui;
}
