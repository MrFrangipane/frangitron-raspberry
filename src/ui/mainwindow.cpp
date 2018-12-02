#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{    
    _setupUi();

    _engineThread = new QThread();
    _engineThread->setObjectName("AudioMidi");
    _engineWorker = new EngineWorker();
    _engineWorker->moveToThread(_engineThread);
    _engineWorker->setStatusCallbacks(this, MainWindow::callbackGetStatus, MainWindow::callbackSetStatus);
    connect(_engineThread, SIGNAL(started()), _engineWorker, SLOT(process()));
    connect(_engineWorker, SIGNAL(finished()), _engineThread, SLOT(quit()));
    connect(_engineWorker, SIGNAL(finished()), _engineWorker, SLOT(deleteLater()));
    connect(_engineWorker, SIGNAL(finished()), _engineThread, SLOT(deleteLater()));
    _engineThread->start();

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

// CALLBACK MECHANISM
EngineStatus MainWindow::callbackGetStatus(void * thisPtr) {
    return ((MainWindow*)thisPtr)->getEngineStatus();
}

void MainWindow::callbackSetStatus(void *thisPtr, EngineStatus status) {
    ((MainWindow*)thisPtr)->setEngineStatus(status);
}

EngineStatus MainWindow::getEngineStatus() const
{
    while( _isWritingStatus ) { }
    return _engineStatus;
}

void MainWindow::setEngineStatus(EngineStatus status)
{
    while( _isReadingStatus ) { }
    _isWritingStatus = true;
    _engineStatus = status;
    _isWritingStatus = false;
}
// ------------------

void MainWindow::_setupUi()
{
    ui->setupUi(this);

    // IN
    _moduleWidgets << new LevelMeterWidget();
    connect(_moduleWidgets[0], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _moduleWidgets[0]->setProperty("displayName", "IN");
    ui->layoutPatch->addWidget(_moduleWidgets[0], 0, 0, 2, 1);

    // IN FILTER
    _moduleWidgets <<  new FilterWidget();
    connect(_moduleWidgets[1], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _moduleWidgets[1]->setProperty("displayName", "IN FILTER");
    ui->layoutPatch->addWidget(_moduleWidgets[1], 0, 1);

    // IN COMP
    _moduleWidgets << new CompWidget();
    connect(_moduleWidgets[2], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _moduleWidgets[2]->setProperty("displayName", "IN COMP");
    ui->layoutPatch->addWidget(_moduleWidgets[2], 1, 1);

    // MASTER
    _moduleWidgets << new LevelMeterWidget();
    connect(_moduleWidgets[3], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _moduleWidgets[3]->setProperty("displayName", "OUT");
    ui->layoutPatch->addWidget(_moduleWidgets[3], 0, 3, 2, 1);

    // END STRETCH
    ui->layoutPatch->addWidget(new QWidget(), 0, ui->layoutPatch->columnCount());
    ui->layoutPatch->setColumnStretch(ui->layoutPatch->columnCount() - 1, 10);

    // HACKY POTTER (Sliders for dev mode) ---
    ui->widgetDevMode->setVisible(false);
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        ui->widgetDevMode->setVisible(true);
        resize(width(), height() + 200);
    } // -------------------------------------
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
    if( _engineStatus.moduleStatuses.empty() ) return;

    while( _isWritingStatus ) { }
    _isReadingStatus = true;
    EngineStatus engineStatus = _engineStatus;
    _isReadingStatus = false;

    // STATUS -> UI
    int i = 0;
    for( AbstractWidget* moduleWidget : _moduleWidgets ) {
        moduleWidget->update_(engineStatus.moduleStatuses[i]);
        i++;
    }

    if( !engineStatus.moduleStatuses[1].empty() )
        engineStatus.moduleStatuses[1]["cutoff"] = ((float)ui->sliderEnc1->value() / 500.0) - 1.0;

    if( !engineStatus.moduleStatuses[2].empty() ) {
        engineStatus.moduleStatuses[2]["threshold"] = ((float)ui->sliderEnc2->value() / 10.0) - 100.0;
        engineStatus.moduleStatuses[2]["ratio"] = fmax((int)_moduleWidgets[2]->isSelected() * 10.0, 1.0);
    }

    // UI -> STATUS
    while( _isWritingStatus ) { }
    _isWritingStatus = true;
    _engineStatus = engineStatus;
    _isWritingStatus = false;
}
