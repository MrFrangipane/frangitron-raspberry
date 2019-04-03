#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(const Configuration *configuration, EngineWorker *engineWorker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    _configuration(configuration),
    _engineWorker(engineWorker)
{   
    _uiStatusHolder = new UiStatusHolder();

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
    delete _uiStatusHolder;
}

void MainWindow::_setupUi()
{
    // SETUP UI, SHOW LOADING WIDGET
    ui->setupUi(this);
    ui->patch->setVisible(false);
    ui->loading->setVisible(true);
    ui->loadingMessage->setText("Loading ...");
#ifdef RASPBERRYPI
    ui->devMode->setVisible(false);
#else
    ui->devMode->setVisible(true);
#endif
    resize(800, 480);

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

#ifndef RASPBERRYPI
    _sliders.push_back(ui->sliderEnc1);
    _sliders.push_back(ui->sliderEnc2);
    _sliders.push_back(ui->sliderEnc3);
    _sliders.push_back(ui->sliderEnc4);
    _sliders.push_back(ui->sliderEnc5);
#endif

    show();
}


void MainWindow::_loadPatch(EngineStatus engineStatus)
{
    // MODULES
    int module = 0;
    for( ConfModule configModule : _configuration->modules )
    {
        // TYPE
        if( configModule.type == std::string("levelMeter") )
            _modules << new LevelMeterWidget();

        else if( configModule.type == std::string("filter") )
            _modules <<  new FilterWidget();

        else if( configModule.type == std::string("compressor") )
            _modules << new CompWidget();

        else if( configModule.type == std::string("kickSynth") )
            _modules << new KickWidget();

        else if( configModule.type == std::string("samplePlayer") )
            _modules << new SamplePlayerWidget(engineStatus.sampleBank);

        else if( configModule.type == std::string("djDeck") )
            _modules << new DjDeckWidget(engineStatus.trackBank);

        // DUMMY
        else if( configModule.type == std::string("dummy") )
        {
            ui->layoutPatch->addWidget(
                new QWidget(),
                configModule.layout.row,
                configModule.layout.col,
                configModule.layout.rowSpan,
                configModule.layout.colSpan
            );

            if( configModule.layout.colStretch != -1 )
                ui->layoutPatch->setColumnStretch(
                    configModule.layout.col,
                    configModule.layout.colStretch
                );

            if( configModule.layout.rowStretch != -1 )
                ui->layoutPatch->setRowStretch(
                    configModule.layout.row,
                    configModule.layout.rowStretch
                );

            continue;
        }

        else continue;

        // WIDGET
        connect(_modules[module], SIGNAL(selectedChanged(bool)), this, SLOT(_selectedChanged()));
        _modules[module]->setProperty("displayName", QString::fromStdString(configModule.name));
        ui->layoutPatch->addWidget(
            _modules[module],
            configModule.layout.row,
            configModule.layout.col,
            configModule.layout.rowSpan,
            configModule.layout.colSpan
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
    return _uiStatusHolder->get();
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
    UiStatus uiStatus = _uiStatusHolder->get();

    // LOADING
    if( engineStatus.status == EngineStatus::LOADING )
    {
        ui->loading->setVisible(true);
        ui->progress->setValue(engineStatus.loadingProgress);
        ui->loadingMessage->setText("Loading ...");
    }
    else if ( engineStatus.status == EngineStatus::MIDI_ERROR )
    {
        ui->loadingMessage->setText("MIDI error");
    }
    else if ( engineStatus.status == EngineStatus::AUDIO_ERROR )
    {
        ui->loadingMessage->setText("Audio error");
    }
    else if ( engineStatus.status == EngineStatus::RUNNING )
    {
        if( !ui->patch->isVisible() ) { // Hacky : should be if( _isPatchLoaded )
            ui->loading->setVisible(false);
            ui->patch->setVisible(true);
            _loadPatch(engineStatus);
        }

        uiStatus.frame += 1;

        // ENGINE STATUS -> INFOS
        ui->sequencer->setStep(int(engineStatus.clock.step));
        ui->labelTime->setText(
            QTime(0,0,0,0).addMSecs(engineStatus.clock.seconds * 1000.0).toString("hh:mm:ss.zzz") +
            QString(" s - ") + QString::number(engineStatus.clock.bar) + QString(".") + QString::number(engineStatus.clock.beat % 4) +
            QString(" bar @ ") +
            QString::number(engineStatus.clock.tempo, 'f', 1)
        );

        // ENGINE STATUS -> MODULE WIDGETS
        int selectedModule = -1;
        int i = 0;
        for( AbstractWidget* moduleWidget : _modules ) {

            moduleWidget->update_(engineStatus.modulesStatuses[i]);

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

                #ifndef RASPBERRYPI
                _sliders[paramId]->setMinimum(0);
                _sliders[paramId]->setMaximum(2);
                _sliders[paramId]->setSingleStep(1);
                _sliders[paramId]->setValue(1);
                _sliders[paramId]->setEnabled(false);

                _previousValues[paramId] = 0.0;
                #endif
            }
        }
        else {
            // NEW SELECTION
            if ( selectedModule != uiStatus.selectedModule ) {
                uiStatus.selectedModule = selectedModule;

                for( int paramId = 0; paramId < MIDI_ENCODER_COUNT; paramId++ ) {
                    if( engineStatus.modulesStatuses[selectedModule].params[paramId].isVisible )
                    {
                        text = QString::fromStdString(engineStatus.modulesStatuses[selectedModule].params[paramId].name);
                        _nameLabels[paramId]->setText(text);

                        #ifndef RASPBERRYPI
                        _sliders[paramId]->setMinimum(engineStatus.modulesStatuses[selectedModule].params[paramId].min * 1000);
                        _sliders[paramId]->setMaximum(engineStatus.modulesStatuses[selectedModule].params[paramId].max * 1000);
                        _sliders[paramId]->setSingleStep(engineStatus.modulesStatuses[selectedModule].params[paramId].step * 1000);
                        _sliders[paramId]->setValue(engineStatus.modulesStatuses[selectedModule].params[paramId].value * 1000);
                        _sliders[paramId]->setEnabled(true);

                        _previousValues[paramId] = _sliders[paramId]->value();
                        #endif
                    }
                    else {
                        _nameLabels[paramId]->setText("");

                        _valueLabels[paramId]->setText("");

                        #ifndef RASPBERRYPI
                        _sliders[paramId]->setMinimum(0);
                        _sliders[paramId]->setMaximum(2);
                        _sliders[paramId]->setSingleStep(1);
                        _sliders[paramId]->setValue(1);
                        _sliders[paramId]->setEnabled(false);

                        _previousValues[paramId] = 0.0;
                        #endif
                    }
                }
            }
            // UPDATE STATUS
            for( int paramId = 0; paramId < MIDI_ENCODER_COUNT; paramId++ ) {
                if( !engineStatus.modulesStatuses[selectedModule].params[paramId].isVisible )
                    continue;

                // ENGINE -> UI
                _valueLabels[paramId]->setText(_modules[selectedModule]->formatParameter(paramId));

                #ifndef RASPBERRYPI
                // SLIDER MOVED
                float sliderValue = _sliders[paramId]->value();

                _sliders[paramId]->setMinimum(engineStatus.modulesStatuses[selectedModule].params[paramId].min * 1000);
                _sliders[paramId]->setMaximum(engineStatus.modulesStatuses[selectedModule].params[paramId].max * 1000);

                if( _previousValues[paramId] != sliderValue ) {
                    uiStatus.paramIncrements[paramId] = (float)(sliderValue - _previousValues[paramId]) / 1000.0;
                    _previousValues[paramId] = sliderValue;
                }
                // SLIDER NOT MOVED
                else {
                    uiStatus.paramIncrements[paramId] = 0.0;
                    _sliders[paramId]->setValue(engineStatus.modulesStatuses[selectedModule].params[paramId].value * 1000);
                    _previousValues[paramId] = _sliders[paramId]->value();
                }
                #endif
            }
        }

        // TEMP -> MEMBERS
        _uiStatusHolder->set(uiStatus);
    }
}

void MainWindow::_stop()
{
    _timerRefresh->stop();
    _engineWorker->stop();

    delete _timerRefresh;
    close();
}
