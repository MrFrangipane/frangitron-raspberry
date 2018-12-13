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

void MainWindow::_setupUi()
{
    ui->setupUi(this);

    _nameLabels.push_back(ui->labelEncName1);
    _nameLabels.push_back(ui->labelEncName2);
    _nameLabels.push_back(ui->labelEncName3);
    _nameLabels.push_back(ui->labelEncName4);
    _nameLabels.push_back(ui->labelEncName5);

    _valueLabels.push_back(ui->labelEncValue1);
    _valueLabels.push_back(ui->labelEncValue2);
    _valueLabels.push_back(ui->labelEncValue3);
    _valueLabels.push_back(ui->labelEncValue4);
    _valueLabels.push_back(ui->labelEncValue5);

    _sliders.push_back(ui->sliderEnc1);
    _sliders.push_back(ui->sliderEnc2);
    _sliders.push_back(ui->sliderEnc3);
    _sliders.push_back(ui->sliderEnc4);
    _sliders.push_back(ui->sliderEnc5);

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

// CALLBACK MECHANISM
UiStatus MainWindow::callbackGetStatus(void * thisPtr) {
    return ((MainWindow*)thisPtr)->getStatus();
}

void MainWindow::callbackSetStatus(void *thisPtr, EngineStatus status) {
    ((MainWindow*)thisPtr)->setEngineStatus(status);
}

UiStatus MainWindow::getStatus()
{
    return _uiStatus;
}

void MainWindow::setEngineStatus(EngineStatus engineStatus)
{
    _engineStatus = engineStatus;
}
// ------------------

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
    // MEMBERS -> TEMP
    EngineStatus engineStatus = _engineStatus;
    UiStatus uiStatus = _uiStatus;

    uiStatus.frame += 1;

    if( engineStatus.modules[0].params[0].name.empty() ) return;

    // ENGINE STATUS -> MODULE WIDGETS
    int selectedModule = -1;
    int i = 0;
    for( AbstractWidget* moduleWidget : _modules ) {

        moduleWidget->update_(engineStatus.modules[i]);

        if( moduleWidget->isSelected() ) selectedModule = i;

        i++;
    }

    // ENGINE STATUS -> PARAMS
    if( selectedModule == -1 ) {
        // NO SELECTION
        uiStatus.selectedModule = -1;

        for( int paramId = 0; paramId < 5; paramId++ ) {
            _nameLabels[paramId]->setText("");

            _valueLabels[paramId]->setText("");

            _sliders[paramId]->setMinimum(0);
            _sliders[paramId]->setMaximum(2);
            _sliders[paramId]->setSingleStep(1);
            _sliders[paramId]->setValue(1);
            _sliders[paramId]->setEnabled(false);

            _previousValues[paramId] = 0.0;
        }
    }
    else {
        // NEW SELECTION
        if ( selectedModule != uiStatus.selectedModule ) {
            uiStatus.selectedModule = selectedModule;

            for( int paramId = 0; paramId < 5; paramId++ ) {
                if( engineStatus.modules[selectedModule].params[paramId].visible )
                {
                    _nameLabels[paramId]->setText(QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name));

                    _sliders[paramId]->setMinimum(engineStatus.modules[selectedModule].params[paramId].min * 1000);
                    _sliders[paramId]->setMaximum(engineStatus.modules[selectedModule].params[paramId].max * 1000);
                    _sliders[paramId]->setSingleStep(engineStatus.modules[selectedModule].params[paramId].step * 1000);
                    _sliders[paramId]->setValue(engineStatus.modules[selectedModule].params[paramId].value * 1000);
                    _sliders[paramId]->setEnabled(true);

                    _previousValues[paramId] = _sliders[paramId]->value();
                }
                else {
                    _nameLabels[paramId]->setText("");

                    _valueLabels[paramId]->setText("");

                    _sliders[paramId]->setMinimum(0);
                    _sliders[paramId]->setMaximum(2);
                    _sliders[paramId]->setSingleStep(1);
                    _sliders[paramId]->setValue(1);
                    _sliders[paramId]->setEnabled(false);

                    _previousValues[paramId] = 0.0;
                }
            }
        }
        for( int paramId = 0; paramId < 5; paramId++ ) {
            if( !engineStatus.modules[selectedModule].params[paramId].visible ) continue;

            // ENGINE -> UI
            if( engineStatus.encoders[paramId].isPressed ) {
                _nameLabels[paramId]->setText(QString("*") + QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name) + QString("*"));
            }
            else {
                _nameLabels[paramId]->setText(QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name));
            }

            _valueLabels[paramId]->setText(_modules[selectedModule]->formatParameter(paramId));

            // SLIDER MOVED
            float sliderValue = _sliders[paramId]->value();
            if( _previousValues[paramId] != sliderValue ) {
                uiStatus.paramIncrements[paramId] = (float)(sliderValue - _previousValues[paramId]) / 1000.0;
                _previousValues[paramId] = sliderValue;
            }
            // SLIDER NOT MOVED
            else {
                uiStatus.paramIncrements[paramId] = 0.0;
                _sliders[paramId]->setValue(engineStatus.modules[selectedModule].params[paramId].value * 1000);
                _previousValues[paramId] = _sliders[paramId]->value();
            }
        }
    }

    // TEMP -> MEMBERS
    _uiStatus = uiStatus;
}
