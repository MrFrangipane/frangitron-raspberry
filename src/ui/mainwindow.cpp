#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(const Configuration *configuration, EngineWorker *engineWorker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    _configuration(configuration),
    _engineWorker(engineWorker)
{
    _engineWorker->setStatusCallbacks(this, MainWindow::callbackGetStatus, MainWindow::callbackSetStatus);

    _setupUi();
    QThread::currentThread()->setPriority(QThread::LowPriority);

    _timerRefresh = new QTimer();
    connect(_timerRefresh, SIGNAL(timeout()), this, SLOT(_refresh()));
    _timerRefresh->start(1000 / UI_FRAMERATE);

    connect(ui->exit, SIGNAL(clicked(bool)), this, SLOT(_stop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_setupUi()
{
    // SETUP UI, SHOW LOADING WIDGET
    ui->setupUi(this);
    ui->patch->setVisible(false);
    ui->loading->setVisible(true);
    ui->devMode->setVisible(false);
    resize(800, 480);

    // HACKY POTTER (Sliders for dev mode) ---
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        ui->devMode->setVisible(true);
    } // -------------------------------------

    // WIDGET LISTS FOR EASY PARAM LOOPING
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

    show();
}


void MainWindow::_loadPatch()
{
    // MODULES
    int module = 0;
    for( int configModule = 0; configModule < MODULE_MAX_COUNT; configModule++ )
    {
        // TYPE
        if( _configuration->modules[configModule].type == std::string("levelMeter") )
            _modules << new LevelMeterWidget();

        else if( _configuration->modules[configModule].type == std::string("filter") )
            _modules <<  new FilterWidget();

        else if( _configuration->modules[configModule].type == std::string("compressor") )
            _modules << new CompWidget();

        else if( _configuration->modules[configModule].type == std::string("kickSynth") )
            _modules << new KickWidget();

        // DUMMY
        else if( _configuration->modules[configModule].type == std::string("dummy") )
        {
            ui->layoutPatch->addWidget(
                new QWidget(),
                _configuration->modules[configModule].layout.row,
                _configuration->modules[configModule].layout.col,
                _configuration->modules[configModule].layout.rowSpan,
                _configuration->modules[configModule].layout.colSpan
            );

            if( _configuration->modules[configModule].layout.colStretch != -1 )
                ui->layoutPatch->setColumnStretch(
                    _configuration->modules[configModule].layout.col,
                    _configuration->modules[configModule].layout.colStretch
                );

            if( _configuration->modules[configModule].layout.rowStretch != -1 )
                ui->layoutPatch->setRowStretch(
                    _configuration->modules[configModule].layout.row,
                    _configuration->modules[configModule].layout.rowStretch
                );

            continue;
        }

        else continue;

        // WIDGET
        connect(_modules[module], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
        _modules[module]->setProperty("displayName", QString::fromStdString(_configuration->modules[configModule].name));
        ui->layoutPatch->addWidget(
            _modules[module],
            _configuration->modules[configModule].layout.row,
            _configuration->modules[configModule].layout.col,
            _configuration->modules[configModule].layout.rowSpan,
            _configuration->modules[configModule].layout.colSpan
        );

        module++;
    }

    // SHOW PATCH
    ui->patch->setVisible(true);
}

// CALLBACK MECHANISM ---
UiStatus MainWindow::callbackGetStatus(void * thisPtr) {
    return ((MainWindow*)thisPtr)->getStatus();
}

void MainWindow::callbackSetStatus(void *thisPtr, EngineStatus status) {
    ((MainWindow*)thisPtr)->setEngineStatus(status);
}

UiStatus MainWindow::getStatus() {
    return _uiStatus;
}

void MainWindow::setEngineStatus(EngineStatus engineStatus) {
    _engineStatus = engineStatus;
}
// ----------------------

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
    QString text;

    // MEMBERS -> TEMP
    EngineStatus engineStatus = _engineStatus;
    UiStatus uiStatus = _uiStatus;

    // LOADING
    if( engineStatus.state == EngineStatus::LOADING )
    {
        ui->loading->setVisible(true);
        ui->progress->setValue(engineStatus.loading_progress);
    }
    else if ( engineStatus.state == EngineStatus::RUNNING )
    {
        if( !ui->patch->isVisible() ) {
            ui->loading->setVisible(false);
            ui->patch->setVisible(true);
            _loadPatch();
        }

        uiStatus.frame += 1;

        // ENGINE STATUS -> INFOS
        ui->sequencer->set_step(engineStatus.clock.sequence_step);
        ui->labelTime->setText(QTime(0,0,0,0).addMSecs(engineStatus.clock.seconds * 1000.0).toString("hh:mm:ss.zzz") + QString(" s ") + QString::number(engineStatus.clock.bar, 'f', 1) + QString(" bar"));

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

            for( int paramId = 0; paramId < MIDI_ENCODER_COUNT; paramId++ ) {
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

                for( int paramId = 0; paramId < MIDI_ENCODER_COUNT; paramId++ ) {
                    if( engineStatus.modules[selectedModule].params[paramId].visible )
                    {
                        text = QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name);
                        _nameLabels[paramId]->setText(text);

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
            // UPDATE STATUS
            for( int paramId = 0; paramId < MIDI_ENCODER_COUNT; paramId++ ) {
                if( !engineStatus.modules[selectedModule].params[paramId].visible ) continue;

                // ENGINE -> UI
                if( engineStatus.encoders[paramId].pressed ) {
                    text = QString("*");
                    text += QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name);
                    text += QString("*");
                    _nameLabels[paramId]->setText(text);
                }
                else {
                    text = QString::fromStdString(engineStatus.modules[selectedModule].params[paramId].name);
                    _nameLabels[paramId]->setText(text);
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
}

void MainWindow::_stop()
{
    _timerRefresh->stop();
    _engineWorker->stop();
    close();
}
