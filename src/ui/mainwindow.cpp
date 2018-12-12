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

EngineStatus MainWindow::getEngineStatus()
{
    while( _isStatusLocked ) { }

    _isStatusLocked = true;
    EngineStatus engineStatus = _engineStatus;
    _isStatusLocked = false;

    return engineStatus;
}

void MainWindow::setEngineStatus(EngineStatus engineStatus)
{
    while( _isStatusLocked ) { }

    _isStatusLocked = true;
    _engineStatus = engineStatus;
    _isStatusLocked = false;
}
// ------------------

void MainWindow::_setupUi()
{
    ui->setupUi(this);

    _paramNames.push_back(ui->labelEncName1);
    _paramNames.push_back(ui->labelEncName2);
    _paramNames.push_back(ui->labelEncName3);
    _paramNames.push_back(ui->labelEncName4);
    _paramNames.push_back(ui->labelEncName5);

    _paramValues.push_back(ui->labelEncValue1);
    _paramValues.push_back(ui->labelEncValue2);
    _paramValues.push_back(ui->labelEncValue3);
    _paramValues.push_back(ui->labelEncValue4);
    _paramValues.push_back(ui->labelEncValue5);

    _paramSliders.push_back(ui->sliderEnc1);
    _paramSliders.push_back(ui->sliderEnc2);
    _paramSliders.push_back(ui->sliderEnc3);
    _paramSliders.push_back(ui->sliderEnc4);
    _paramSliders.push_back(ui->sliderEnc5);

    // IN
    _modules << new LevelMeterWidget();
    connect(_modules[0], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _modules[0]->setProperty("displayName", "IN");
    ui->layoutPatch->addWidget(_modules[0], 0, 0, 2, 1);

    // IN FILTER
    _modules <<  new FilterWidget();
    connect(_modules[1], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _modules[1]->setProperty("displayName", "IN FILTER");
    ui->layoutPatch->addWidget(_modules[1], 0, 1);

    // IN COMP
    _modules << new CompWidget();
    connect(_modules[2], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _modules[2]->setProperty("displayName", "IN COMP");
    ui->layoutPatch->addWidget(_modules[2], 1, 1);

    // MASTER
    _modules << new LevelMeterWidget();
    connect(_modules[3], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
    _modules[3]->setProperty("displayName", "OUT");
    ui->layoutPatch->addWidget(_modules[3], 0, 3, 2, 1);

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
    for( AbstractWidget* moduleWidget : _modules ) {
        if( moduleWidget != sender() ) {
            moduleWidget->desselect();
        }
    }
}

void MainWindow::_refresh()
{
    if( _engineStatus.modules[0].params[0].name.empty() ) return;

    while( _isStatusLocked ) { }

    _isStatusLocked = true;
    EngineStatus engineStatus = _engineStatus;
    _isStatusLocked = false;

    // STATUS -> UI
    int selectedModule = -1;
    int i = 0;
    for( AbstractWidget* moduleWidget : _modules ) {

        moduleWidget->update_(engineStatus.modules[i]);

        if( moduleWidget->isSelected() ) selectedModule = i;

        i++;
    }

    // PARAMS
    if( selectedModule == -1 ) {
        engineStatus.selectedModule = -1;

        for( int paramId = 0; paramId < 5; paramId++ ) {
            _paramNames[paramId]->setText("");

            _paramValues[paramId]->setText("");

            _paramSliders[paramId]->setMinimum(0);
            _paramSliders[paramId]->setMaximum(2);
            _paramSliders[paramId]->setSingleStep(1);
            _paramSliders[paramId]->setValue(1);
            _paramSliders[paramId]->setEnabled(false);
        }
    }
    else if ( selectedModule != engineStatus.selectedModule ) {
        engineStatus.selectedModule = selectedModule;

        for( int paramId = 0; paramId < 5; paramId++ ) {
            if( engineStatus.modules[selectedModule].params[paramId].visible )
            {
                _paramNames[paramId]->setText(QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name));

                _paramSliders[paramId]->setMinimum(engineStatus.modules[selectedModule].params[paramId].min * 1000);
                _paramSliders[paramId]->setMaximum(engineStatus.modules[selectedModule].params[paramId].max * 1000);
                _paramSliders[paramId]->setSingleStep(engineStatus.modules[selectedModule].params[paramId].step * 1000);
                _paramSliders[paramId]->setValue(engineStatus.modules[selectedModule].params[paramId].value * 1000);
                _paramSliders[paramId]->setEnabled(true);
            }
            else {
                _paramNames[paramId]->setText("");

                _paramValues[paramId]->setText("");

                _paramSliders[paramId]->setMinimum(0);
                _paramSliders[paramId]->setMaximum(2);
                _paramSliders[paramId]->setSingleStep(1);
                _paramSliders[paramId]->setValue(1);
                _paramSliders[paramId]->setEnabled(false);
            }
        }
    }
    else if( selectedModule == engineStatus.selectedModule ) {
        for( int paramId = 0; paramId < 5; paramId++ ) {
            if( !engineStatus.modules[selectedModule].params[paramId].visible ) continue;

            if( engineStatus.encoders[paramId].isPressed ) {
                _paramNames[paramId]->setText(QString("*") + QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name) + QString("*"));
            }
            else {
                _paramNames[paramId]->setText(QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name));
            }

            engineStatus.modules[selectedModule].params[paramId].value = (float)_paramSliders[paramId]->value() / 1000.0;
            _paramValues[paramId]->setText(_modules[selectedModule]->formatParameter(paramId));
        }
    }

    // UI -> STATUS
    while( _isStatusLocked ) { }

    _isStatusLocked = true;
    _engineStatus = engineStatus;
    _isStatusLocked = false;
}
