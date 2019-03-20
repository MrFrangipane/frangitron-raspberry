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
                _midiDeviceIndex = i;
                delete midiIn;
                return;
            }
        }
    }

    delete midiIn;
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

    // DJ TRACK BANK
    for( AudioFileInfos audioFileInfos : _configuration->djTracks ) {
        if( audioFileInfos.filepath.empty() ) // Exit at first empty clip
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
            _shared.patch.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));

        else if( configModule.type == std::string("filter") )
            _shared.patch.push_back(std::make_shared<Filter>(Filter(_bufferSize)));

        else if( configModule.type == std::string("compressor") )
            _shared.patch.push_back(std::make_shared<Compressor>(Compressor(_bufferSize)));

        else if( configModule.type == std::string("kickSynth") )
            _shared.patch.push_back(std::make_shared<KickSynth>(KickSynth(_bufferSize)));

        else if( configModule.type == std::string("samplePlayer") )
            _shared.patch.push_back(std::make_shared<SamplePlayer>(SamplePlayer(_bufferSize, _shared.engine.sampleBank)));

        else if( configModule.type == std::string("djDeck") )
        {
            DjDeckInfos djDeckInfos;
            djDeckInfos.name = configModule.name;
            djDeckInfos.moduleIndex = _shared.patch.size();

            _shared.engine.trackBank->registerDjDeck(djDeckInfos);
            _shared.patch.push_back(std::make_shared<DjDeck>(DjDeck(djDeckInfos, _bufferSize)));
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

    // MIDI DEVICE
    _setMidiDeviceIndex();
    try {
        _midi = new RtMidiIn();
    }
    catch( RtMidiError &error ) {
        std::cout << "Error while allocating Midi" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::AUDIO_ERROR;
        _shared.uiSetStatus(_shared.uiPtr, _shared.engine);
        return;
    }

    try {
        std::cout << "Opening midi port (" << _midiDeviceIndex << ") " <<
                     "and setting callback : " << _midi->getPortName(_midiDeviceIndex) << std::endl;

        _midi->openPort(_midiDeviceIndex);

        _midi->setCallback(&_midiCallback, &_shared);
        _midi->ignoreTypes(false, false, false);
    }
    catch( RtMidiError &error ) {
        std::cout << "Error while opening midi port and setting callback" << std::endl;
        std::cout << error.getMessage() << std::endl;
        _shared.engine.status = EngineStatus::AUDIO_ERROR;
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
        if( audioFileInfos.filepath.empty() ) // Exit at first empty clip
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
    delete _midi;
}

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    int moduleId = 0;
    int inputId = -2;
    int masterId = 0;
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
    UiStatus uiStatus = s->uiStatus(s->uiPtr);
    s->engine.selectedModule = uiStatus.selectedModule;
    if( s->engine.selectedModule != -1 )
    {
        // UI SLIDERS -> ENGINE
        #ifndef RASPBERRYPI
        if( s->uiFrame != uiStatus.frame ) {
            s->uiFrame = uiStatus.frame;

            int paramId = 0;
            for( ModuleParameter moduleParameter : s->engine.modulesStatuses[s->engine.selectedModule].params )
            {
                if( paramId >= MIDI_ENCODER_COUNT ) break;
                if( !moduleParameter.isVisible ) {
                    paramId++;
                    continue;
                }

                float increment = uiStatus.paramIncrements[paramId];

                s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].value = fmax(
                    moduleParameter.min, fmin(moduleParameter.value + increment, moduleParameter.max)
                );

                paramId++;
            }
        }
        #endif

        // MIDI ENCODERS -> ENGINE
        int paramId = 0;
        float increment = 0;
        bool pressed = false;
        for( ModuleParameter parameter : s->engine.modulesStatuses[s->engine.selectedModule].params)
        {
            if( paramId >= MIDI_ENCODER_COUNT ) break;

            pressed = s->midiEncoders[paramId].pressed(s->time.engineFrame());
            increment = s->midiEncoders[paramId].increment(s->time.engineFrame());
            s->engine.encoders[paramId].pressed = pressed;

            if( increment != 0 )
            {
                increment *= parameter.step;
                increment *= ((int)pressed * (MIDI_PUSHED_FACTOR - 1)) + 1;

                s->engine.modulesStatuses[s->engine.selectedModule].params[paramId].value = fmax(
                    parameter.min, fmin(parameter.value + increment, parameter.max)
                );

                s->midiEncoders[paramId].setIncrement(0, s->time.engineFrame());
            }

            paramId++;
        }
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
            s->patch[note.moduleIndex]->gate(s->time.engineFrame());
    }
    // TODO : use note off from midi ?
    for( SubscribedNote note : s->subscribedNotes )
        s->midiNoteOn[note.noteNumber] = false;

    // PATCH PROCESS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->patch ) {
        inputId = s->patchWires[moduleId];

        if( inputId == -1 ) {  // Hardware Input
            module->process(ioIn, s->time.engineFrame());
        }
        else if( inputId >= 0 ) {  // Module Input
           module->process(s->patch[inputId]->output(), s->time.engineFrame());
        }

        moduleId++;
    }


    // HACKY DJTRACKBANK TEST ---
    /*
    DjDeckInfos deck = s->djTrackBank->deckInfos(0);
    if( (int)s->engine.modulesStatuses[2].params[2].value - 1 != deck.trackIndex )
    {
        deck.trackIndex = (int)s->engine.modulesStatuses[2].params[2].value - 1;
        deck.needsLoading = true;
        s->djTrackBank->setDeckInfos(0, deck);
    }

    ConfAudioFile track = s->djTrackBank->trackInfos(deck.trackIndex);
    int sampleIndex = 0;
    for( nSample i = 0; i < bufferSize * CHANNEL_COUNT; i++ ) {
        sampleIndex = (s->time.engineFrame() * CHANNEL_COUNT + i) % (track.frameCount * track.channelCount);
        ioOut[i] = s->djTrackBank->sample(0, sampleIndex);
    }
    */
    // --------------------------

    // PATCH -> OUTPUT
    masterId = s->patch.size() - 1;
    for( nSample i = 0; i < bufferSize * CHANNEL_COUNT; i++ ) {
        ioOut[i] = s->patch[masterId]->output()[i];
    }

    // OUTPUT -> RECORDER
    s->recorder->write(ioOut);

    // PATCH -> ENGINE
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->patch ) {
        s->engine.modulesStatuses[moduleId] = module->status();
        moduleId++;
    }

    // INCREMENT TIME
    s->engine.clock.bar = s->time.bar();
    s->engine.clock.seconds = s->time.seconds();
    s->engine.clock.sequenceStep = s->time.sequenceStep();
    s->engine.clock.isPlaying = s->time.isPlaying();
    s->time.incrementFrame(bufferSize);

    // ENGINE -> UI
    // TODO : clarify terms between UiStatus, EngineStatus, ...
    s->uiSetStatus(s->uiPtr, s->engine);

    return 0;
}

void Engine::_midiCallback(double /*deltaTime*/, std::vector<unsigned char> *message, void *userData)
{
    int encoder = 0;
    Shared* shared = (Shared*)userData;

    if( shared->engine.status != EngineStatus::RUNNING ) return;

//     DEBUG COUT
//    unsigned int nBytes = message->size();
//    for ( unsigned int i=0; i<nBytes; i++ )
//      std::cout << "Byte " << i << " = " << (int)message->at(i) << ", " << std::endl;

    // TIMING
    if( message->at(0) == 250 ) shared->time.start();
    if( message->at(0) == 252 ) shared->time.stop();
    if( message->at(0) == 251 ) shared->time.resume();

    if( message->at(0) == 248 && shared->time.isPlaying() ) { // Midi Clock Pulse
        shared->time.incrementPpqn(1);
    }

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
                shared->midiEncoders[encoder].setPressed((bool)((int)message->at(2) / 127), shared->time.engineFrame());
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
                shared->midiEncoders[encoder].setIncrement(-1, shared->time.engineFrame());

                shared->midiMsb = -1;
                shared->midiLsb = -1;
            break;

            // INCREASE
            case 96:
                encoder = (shared->midiLsb << 7) | shared->midiMsb;
                shared->midiEncoders[encoder].setIncrement(1, shared->time.engineFrame());

                shared->midiMsb = -1;
                shared->midiLsb = -1;
            break;

            default:
            break;
        }
    }
}
