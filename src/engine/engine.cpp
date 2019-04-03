#include "engine.h"


Engine::Engine(const Configuration *configuration) :
    _configuration(configuration)
{
    for( int moduleIndex = 0; moduleIndex < MODULE_MAX_COUNT; moduleIndex++ )
    {
        _shared.engine.modulesStatuses[moduleIndex].isDummy = true;
    }
}

void Engine::_setAudioDeviceIndex()
{
    std::vector<std::string> interfaceNames;
    interfaceNames.push_back("pisound");
    interfaceNames.push_back("Fireface");

    RtAudio* audio;

    try {
        audio = new RtAudio();
    }
    catch( RtAudioError &error ) {
        std::cout << "Engine : Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    std::cout << std::endl << "Available audio devices :" << std::endl;

    RtAudio::DeviceInfo deviceInfos;

    for( unsigned int i = 0; i < audio->getDeviceCount(); i++ ) {

        deviceInfos = audio->getDeviceInfo(i);
        std::cout << i << " : " << deviceInfos.name <<
                    " duplex:" << deviceInfos.duplexChannels <<
                    " in:" << deviceInfos.inputChannels <<
                    " out:" << deviceInfos.outputChannels <<
                     std::endl;

        for( std::string interfaceName : interfaceNames) {

            if( deviceInfos.inputChannels != 0 &&
                deviceInfos.outputChannels != 0 &&
                deviceInfos.name.find(interfaceName) != std::string::npos )
            {
                    _audioDeviceIndex = i;
                    delete audio;
                    return;
            }
        }
    }

    delete audio;
    _audioDeviceIndex = 0;
}

void Engine::_setMidiDeviceIndex()
{
    std::vector<std::string> interfaceNames;
    interfaceNames.push_back("MIOS");
    interfaceNames.push_back("Fireface");

    RtMidiIn* midiIn;

    try {
        midiIn = new RtMidiIn();
    }
    catch( RtMidiError &error ) {
        std::cout << "Engine : Error while allocating Midi" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    std::cout << std::endl << "Available midi devices :" << std::endl;

    std::string portName;

    for( unsigned int i = 0; i < midiIn->getPortCount(); i++ ) {
        try {
            portName = midiIn->getPortName(i);
        }
        catch( RtMidiError &error ) {
            std::cout << "Engine : Error while opening Midi port" << std::endl;
            std::cout << error.getMessage() << std::endl;
            continue;
        }

        std::cout << i << " : " << portName << std::endl;

        for( std::string interfaceName : interfaceNames ) {
            if( portName.find(interfaceName) != std::string::npos )
            {
                // TODO : unify data structures !!
                _midiDeviceIndex = i;
                _shared.midiPortNumber = i;
                delete midiIn;
                return;
            }
        }
    }

    delete midiIn;
    _shared.midiPortNumber = 0;
    _midiDeviceIndex = 0;
}

void Engine::start()
{
    // STATE <- LOADING
    _shared.engine.status = EngineStatus::LOADING;
    _shared.engine.sampleBank = new SampleBank();
    _shared.engine.trackBank = new DjTrackBank();

    // WAIT FOR UI CALLBACK REGISTRATION
    while ( _shared.uiPtr == nullptr ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // HACKY POTTER (Fireface UCX in ClassCompilant mode on Ubuntu) ---
    #ifndef RASPBERRYPI
    _bufferSize = 60;
    #endif
    // --------------------------------------------------------------

    // BUFFERS
    _shared.engine.silence.resize(_bufferSize * CHANNEL_COUNT);
    _shared.engine.masterBus.resize(_bufferSize * CHANNEL_COUNT);

    // DJ TRACK BANK
    for( AudioFileInfos audioFileInfos : _configuration->djTracks ) {
        if( audioFileInfos.filepath.empty() )
            break;

        _shared.engine.trackBank->registerAudioFile(audioFileInfos);
    }
    _shared.engine.trackBank->start();

    // PATCH LOADING (AUDIO MODULES)
    int module = 0;
    for( ConfModule configModule : _configuration->modules )
    {
        // TYPE
        if( configModule.type == std::string("levelMeter") )
            _shared.patch.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize, configModule.routedToMasterBus)));

        else if( configModule.type == std::string("filter") )
            _shared.patch.push_back(std::make_shared<Filter>(Filter(_bufferSize, configModule.routedToMasterBus)));

        else if( configModule.type == std::string("compressor") )
            _shared.patch.push_back(std::make_shared<Compressor>(Compressor(_bufferSize, configModule.routedToMasterBus)));

        else if( configModule.type == std::string("kickSynth") )
            _shared.patch.push_back(std::make_shared<KickSynth>(KickSynth(_bufferSize, configModule.routedToMasterBus)));

        else if( configModule.type == std::string("samplePlayer") )
            _shared.patch.push_back(std::make_shared<SamplePlayer>(SamplePlayer(_bufferSize, _shared.engine.sampleBank, configModule.routedToMasterBus)));

        else if( configModule.type == std::string("djDeck") )
        {
            DjDeckInfos djDeckInfos;
            djDeckInfos.name = configModule.name;
            djDeckInfos.moduleIndex = _shared.patch.size();

            djDeckInfos = _shared.engine.trackBank->registerDjDeck(djDeckInfos);
            _shared.patch.push_back(std::make_shared<DjDeck>(DjDeck(djDeckInfos, _bufferSize, _shared.engine.trackBank, configModule.routedToMasterBus)));
        }

        else continue;

        // WIRE
        _shared.patchWires.push_back(configModule.inputIndex);

        // REGISTER MIDI NOTE
        if( configModule.midiNote != -1 )
            _shared.subscribedNotes.push_back(SubscribedNote(configModule.midiNote, module));

        // STATUS & OVERRIDES
        _shared.engine.modulesStatuses[module] = _shared.patch[module]->status();

        int paramIndex = 0;
        for( ConfModuleParamOverride override : configModule.overrides )
        {
            if( override.isActive )
                _shared.engine.modulesStatuses[module].params[paramIndex].value = override.value;

            paramIndex++;
        }

        module++;
    }

    // MIDI IN
    _setMidiDeviceIndex();
    try {
        _midiIn = new RtMidiIn();
    }
    catch( RtMidiError &error ) {
        std::cout << "Error while allocating Midi in" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::MIDI_ERROR;
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);
        return;
    }

    try {
        std::cout << "Opening midi in port (" << _midiDeviceIndex << ") " <<
                     "and setting callback : " << _midiIn->getPortName(_midiDeviceIndex) << std::endl;

        _midiIn->openPort(_midiDeviceIndex);

        _midiIn->setCallback(&_midiInCallback, &_shared);
        _midiIn->ignoreTypes(false, false, false);
    }
    catch( RtMidiError &error ) {
        std::cout << "Error while opening midi in port and setting callback" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::MIDI_ERROR;
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);
        return;
    }

    // AUDIO DEVICE
    _setAudioDeviceIndex();
    try {
        _audio = new RtAudio();
    }
    catch( RtAudioError &error ) {
        std::cout << "Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::AUDIO_ERROR;
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);
        return;
    }

    RtAudio::StreamParameters _audioInParams;
    _audioInParams.deviceId = _audioDeviceIndex;
    _audioInParams.nChannels = CHANNEL_COUNT;

    RtAudio::StreamParameters _audioOutParams;
    _audioOutParams.deviceId = _audioDeviceIndex;
    _audioOutParams.nChannels = CHANNEL_COUNT;

    try {
        std::cout << "Opening audio device " << "(" << _audioDeviceIndex << ") " <<
                     "and setting callback : " << _audio->getDeviceInfo(_audioDeviceIndex).name << std::endl;

        RtAudio::StreamOptions options;
        options.flags |= RTAUDIO_SCHEDULE_REALTIME;
        options.flags |= RTAUDIO_MINIMIZE_LATENCY;

        _audio->openStream(
            &_audioOutParams,
            &_audioInParams,
            RTAUDIO_FLOAT32, // PiSound supports only up to 32 bits
            SAMPLE_RATE,
            &_bufferSize,
            &_audioCallback,
            &_shared,
            &options
        );

        _audio->startStream();
    }
    catch( RtAudioError &error ) {
        std::cout << "Error while opening audio device and setting callback" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::AUDIO_ERROR;
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);
    }

    // AUDIO FILES
    for( AudioFileInfos audioFileInfos: _configuration->samples )
    {
        if( audioFileInfos.filepath.empty() )
            break;

        SndfileHandle f_audio = SndfileHandle(audioFileInfos.filepath);
        audioFileInfos.frameCount = f_audio.frames();
        audioFileInfos.channelCount = f_audio.channels();

        _shared.engine.sampleBank->registerAudioFile(audioFileInfos);
    }

    for( SampleInfos sampleInfos : _shared.engine.sampleBank->registeredSamples() )
    {
        SndfileHandle f_audio = SndfileHandle(sampleInfos.filepath);
        f_audio.read(
            _shared.engine.sampleBank->pointerToSample(sampleInfos, true),
            sampleInfos.frameCount * sampleInfos.channelCount
        );

        _shared.engine.loadingProgress = _shared.engine.sampleBank->loadingProgress();
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);

        std::cout << "Loaded " << sampleInfos.filepath << std::endl;
    }

    // RECORDER
    _shared.recorder = new Recorder(_bufferSize, RECORDER_CACHE_BUFFER_COUNT);
    _shared.recorder->start();

    // STATE <- RUNNING
    _shared.engine.status = EngineStatus::RUNNING;

    // TEMPO
    _shared.time.setTempo(_configuration->tempo);

    // MIDI OUT
    _midiOutThread = std::thread(_midiOutLoop, &_shared);
    _midiOutThread.detach();
}

void Engine::stop()
{
    // STATE
    _shared.engine.status = EngineStatus::STOPPED;

    // RECORDER
    _shared.recorder->stop();

    // AUDIO
    try {
      _audio->stopStream();
    }
    catch (RtAudioError& e) {
      e.printMessage();
    }
    if ( _audio->isStreamOpen() ) _audio->closeStream();

    // MIDI
    delete _midiIn;
}

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    int moduleId = 0;
    int inputId = -2;
    Sample * ioIn = (Sample*)bufferIn;
    Sample * ioOut = (Sample*)bufferOut;
    Shared * s = (Shared*)userData;

    // ENGINE RUNNING ?
    if( s->engine.status != EngineStatus::RUNNING )
    {
        for( nFrame i = 0; i < bufferSize * CHANNEL_COUNT; i++ )
            ioOut[i] = 0.0;

        return 0;
    }

    // SELECTED MODULE -> ENGINE
    int paramId = 0;
    float increment = 0;
    bool pressed = false;
    bool clicked = false;

    UiStatus uiStatus = s->uiStatus(s->uiPtr);

    // SELECTION CHANGED
    if( s->engine.selectedModule != uiStatus.selectedModule)
    {
        s->engine.selectedModule = uiStatus.selectedModule;
        for( int i = 0; i < MIDI_ENCODER_COUNT; i++ )
            s->midiEncoders[i].reset();
    }

    // SOMETHING SELECTED
    if( s->engine.selectedModule != -1 )
    {
        // UI SLIDERS -> ENGINE ---
        #ifndef RASPBERRYPI
        if( s->uiFrame != uiStatus.frame ) {
            s->uiFrame = uiStatus.frame;

            paramId = 0;
            for( ModuleParameter parameter : s->engine.modulesStatuses[s->engine.selectedModule].params )
            {
                if( paramId >= MIDI_ENCODER_COUNT ) break;
                if( !parameter.isVisible ) {
                    paramId++;
                    continue;
                }

                pressed = uiStatus.paramPressed[paramId];
                clicked = uiStatus.paramClicked[paramId];
                increment = uiStatus.paramIncrements[paramId];

                s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].pressed = pressed;
                s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].clicked = clicked;
                s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].value = fmax(
                    parameter.min, fmin(parameter.value + increment, parameter.max)
                );

                paramId++;
            }
        }
        #endif
        // ------------------------
    }
    // MIDI ENCODERS -> ENGINE
    paramId = 0;
    for( ModuleParameter parameter : s->engine.modulesStatuses[s->engine.selectedModule].params )
    {
        if( paramId >= MIDI_ENCODER_COUNT ) break;
        if( !parameter.isVisible ) {
            paramId++;
            continue;
        }

        pressed = s->midiEncoders[paramId].pressed(s->time.status());
        clicked = s->midiEncoders[paramId].clicked(s->time.status());
        increment = s->midiEncoders[paramId].increment(s->time.status()) * parameter.step;

        s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].pressed = pressed;
        s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].clicked = clicked;
        s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].value = fmax(
            parameter.min, fmin(parameter.value + increment, parameter.max)
        );

        s->midiEncoders[paramId].setIncrement(0, s->time.status());

        paramId++;
    }

    // ENGINE -> PATCH
    moduleId = 0;
    for( ModuleStatus status : s->engine.modulesStatuses ) {
        if( !status.isDummy )
            s->patch[moduleId]->update(status);
        moduleId++;
    }

    // NOTE ON -> PATCH
    for( SubscribedNote note : s->subscribedNotes ) {
        if( s->midiNoteOn[note.noteNumber] )
            s->patch[note.moduleIndex]->gate(s->time.status());
    }
    // TODO : use note off from midi ?
    for( SubscribedNote note : s->subscribedNotes )
        s->midiNoteOn[note.noteNumber] = false;

    // PATCH PROCESS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->patch ) {
        inputId = s->patchWires[moduleId];

        if( inputId == MODULE_INPUT_SOUNDCARD ) {
            module->process(ioIn, s->time.status());
        }
        else if( inputId == MODULE_INPUT_NONE )
        {
           module->process(s->engine.silence.data(), s->time.status());
        }
        else if( inputId == MODULE_INPUT_MASTER_BUS )
        {
           module->process(s->engine.masterBus.data(), s->time.status());
        }
        else if( inputId >= 0 ) {  // Module Input
           module->process(s->patch[inputId]->output(), s->time.status());
        }

        if( module->isRoutedToMasterBus )
        {
            for( nSample i = 0; i < bufferSize * CHANNEL_COUNT; i++ )
            {
               s->engine.masterBus[i] += module->output()[i];
            }
        }

        moduleId++;
    }

    // PATCH -> OUTPUT
    for( nSample i = 0; i < bufferSize * CHANNEL_COUNT; i++ ) {
        ioOut[i] = s->patch.back()->output()[i];
    }    
    std::fill(s->engine.masterBus.begin(), s->engine.masterBus.end(), 0.0);

    // OUTPUT -> RECORDER
    s->recorder->write(ioOut);

    // PATCH -> ENGINE
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->patch ) {
        s->engine.modulesStatuses[moduleId] = module->status();
        moduleId++;
    }

    // INCREMENT TIME
    s->engine.clock = s->time.status();
    s->time.incrementFrame(bufferSize);

    // ENGINE -> UI
    // TODO : clarify terms between UiStatus, EngineStatus, ...
    s->uiSetStatus(s->uiPtr, s->engine);

    return 0;
}

void Engine::_midiInCallback(double /*deltaTime*/, std::vector<unsigned char> *message, void *userData)
{
    int encoder = 0;
    Shared* shared = (Shared*)userData;

    if( shared->engine.status != EngineStatus::RUNNING ) return;

#ifdef MIDI_DEBUG
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
      std::cout << "Byte " << i << " = " << (int)message->at(i) << ", " << std::endl;
#endif

    // TIMING
    if( message->at(0) == 250 ) shared->time.start();
    if( message->at(0) == 252 ) shared->time.stop();
    if( message->at(0) == 251 ) shared->time.resume();

    // NOTE ON
    if( message->at(0) >= 144 && message->at(0) <= 159) {
        //std::cout << "Note on : " << (int)message->at(1) << " vel: " << (int)message->at(2) << " chan: " << (int)message->at(0) - 143 << std::endl;
        if( (int)message->at(2) )
            shared->midiNoteOn[(int)message->at(1)] = true;
    }

    // CONTROL CHANGE
    if( message->at(0) >= 176 && message->at(0) <= 191) {

        switch( (int)message->at(1) ) {

            // PUSHES
            case 20: {}
            case 21: {}
            case 22: {}
            case 23: {}
            case 24:
                encoder = (int)message->at(1) - 20;
                shared->midiEncoders[encoder].setPressed((bool)((int)message->at(2) / 127), shared->time.status());
            break;

            // NRPN MSB
            case 99:
                shared->midiMsb = (int)message->at(2);
            break;

            // NRPN LSB
            case 98:
                shared->midiLsb = (int)message->at(2);
            break;

            // DECREASE
            case 97:
                encoder = (shared->midiLsb << 7) | shared->midiMsb;
                shared->midiEncoders[encoder].setIncrement(-1, shared->time.status());

                shared->midiMsb = -1;
                shared->midiLsb = -1;
            break;

            // INCREASE
            case 96:
                encoder = (shared->midiLsb << 7) | shared->midiMsb;
                shared->midiEncoders[encoder].setIncrement(1, shared->time.status());

                shared->midiMsb = -1;
                shared->midiLsb = -1;
            break;

            default:
            break;
        }
    }
}

void Engine::_midiOutLoop(Shared *shared)
{
    RtMidiOut midi;
    midi.openPort(shared->midiPortNumber);

    std::vector<unsigned char> clockPulseMessage;
    clockPulseMessage.push_back(248);

    ClockStatus clock;
    nPulse lastPulse = 0;

    auto sleepDuration = std::chrono::milliseconds(MIDI_OUT_LOOP_SLEEP);

    while( shared->engine.status == EngineStatus::RUNNING )
    {
        clock = shared->time.status();
        if( clock.enginePulseLatencyCompensated > lastPulse )
        {
            lastPulse = clock.enginePulseLatencyCompensated;
            midi.sendMessage(&clockPulseMessage);
        }
        std::this_thread::sleep_for(sleepDuration);
    }
}
