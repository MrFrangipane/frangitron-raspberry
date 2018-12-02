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
    _audioWorker->setStatusCallback(this, MainWindow::engineStatusCallback);
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
    _moduleWidgets[3]->setProperty("displayName", "MASTER");
    ui->layoutPatch->addWidget(_moduleWidgets[3], 0, 3, 2, 1);

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
    _engineStatus = _audioWorker->status();
    if( _engineStatus.moduleStatuses.empty() ) return;

    // STATUS -> UI
    int i = 0;
    for( AbstractWidget* moduleWidget : _moduleWidgets ) {
        moduleWidget->update_(_engineStatus.moduleStatuses[i]);
        i++;
    }

    // UI -> STATUS
    if( !_engineStatus.moduleStatuses[1].empty() )
        _engineStatus.moduleStatuses[1]["cutoff"] = ((float)ui->sliderEnc1->value() / 500.0) - 1.0;

    if( !_engineStatus.moduleStatuses[2].empty() ) {
        _engineStatus.moduleStatuses[2]["threshold"] = ((float)ui->sliderEnc2->value() / 10.0) - 100.0;
        _engineStatus.moduleStatuses[2]["ratio"] = fmax((int)_moduleWidgets[2]->isSelected() * 10.0, 1.0);
    }
}
