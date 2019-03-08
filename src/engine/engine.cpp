#include "engine.h"

void Engine::_setAudioDeviceIndex()
{
    std::vector<std::string> interfaceNames;
    interfaceNames.push_back("pisound");
    interfaceNames.push_back("Fireface");

    RtAudio* audio;

    try {
        audio = new RtAudio();
    }
    catch (RtAudioError &error) {
        std::cout << "Engine : Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    std::cout << std::endl << "Available audio devices :" << std::endl;

    RtAudio::DeviceInfo device_infos;

    for (unsigned int i = 0; i < audio->getDeviceCount(); i++) {

        device_infos = audio->getDeviceInfo(i);
        std::cout << i << " : " << device_infos.name <<
                    " duplex:" << device_infos.duplexChannels <<
                    " in:" << device_infos.inputChannels <<
                    " out:" << device_infos.outputChannels <<
                     std::endl;

        for (auto interfaceName = interfaceNames.begin(); interfaceName != interfaceNames.end(); interfaceName++) {

            if (device_infos.inputChannels != 0 &&
                device_infos.outputChannels != 0 &&
                device_infos.name.find(*interfaceName) != std::string::npos) {
                    _audioDeviceIndex = i;
                    return;
            }
        }
    }

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

        for (auto interfaceName = interfaceNames.begin(); interfaceName != interfaceNames.end(); interfaceName++) {
            if( portName.find(*interfaceName) != std::string::npos ) {
                _midiDeviceIndex = i;
                return;
            }
        }
    }

    _midiDeviceIndex = 0;
}

void Engine::start()
{
    // HACKY POTTER (to work with Fireface UCX) ---
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        _bufferSize = 60;
    } // ------------------------------------------

    // MODULES
    // INPUT
    _shared.audioModules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.modules[0] = _shared.audioModules[0]->status();
    _shared.status.modules[0].params[10].value = 1.0; // LockLevel
    _shared.audioWires.push_back(-1);
    // FILTER
    _shared.audioModules.push_back(std::make_shared<Filter>(Filter(_bufferSize)));
    _shared.status.modules[1] = _shared.audioModules[1]->status();
    _shared.audioWires.push_back(0);
    // COMP
    _shared.audioModules.push_back(std::make_shared<Compressor>(Compressor(_bufferSize)));
    _shared.status.modules[2] = _shared.audioModules[2]->status();
    _shared.audioWires.push_back(1);
    // KICK SYNTH
    _shared.audioModules.push_back(std::make_shared<KickSynth>(KickSynth(_bufferSize)));
    _shared.status.modules[3] = _shared.audioModules[3]->status();
    _shared.audioWires.push_back(2);
    // OUT
    _shared.audioModules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.modules[4] = _shared.audioModules[4]->status();
    _shared.audioWires.push_back(3);

    // MIDI DEVICE
    _setMidiDeviceIndex();
    try {
        _midi = new RtMidiIn();
    }
    catch( RtMidiError &error ) {
        std::cout << "Error while allocating Midi" << std::endl;
        std::cout << error.getMessage() << std::endl;
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
        return;
    }

    RtAudio::StreamParameters _audioInParams;
    _audioInParams.deviceId = _audioDeviceIndex;
    _audioInParams.nChannels = 2;

    RtAudio::StreamParameters _audioOutParams;
    _audioOutParams.deviceId = _audioDeviceIndex;
    _audioOutParams.nChannels = 2;

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
    }

    // RECORDER
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "/var/frangitron/frangitron_%d-%m-%Y_%H.%M.%S.wav");
    auto filepath = oss.str();

    _shared.recorder= new Recorder(
        _bufferSize,
        RECORDER_CACHE_BUFFER_COUNT,
        filepath
    );
    _shared.recorder->start(_shared.recorder);

    // READY
    _shared.ready = true;
}

void Engine::stop() {
    // READY
    _shared.ready = false;

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
    // INIT
    int moduleId = 0;
    int inputId = -2;
    int masterId = 0;
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    Shared* s = (Shared*)userData;

    // READY ?
    if( !s->ready ){
        for( nFrame i = 0; i < bufferSize * 2; i++ ) {
            ioOut[i] = 0.0;
        }

        return 0;
    }

    // UI STATUS
    UiStatus uiStatus = s->uiGetStatus(s->uiPtr);
    // Selected Module
    if( uiStatus.selectedModule != -1 ) {
        if( s->uiPreviousFrame != uiStatus.frame ) {
            s->uiPreviousFrame = uiStatus.frame;

            s->status.selectedModule = uiStatus.selectedModule;

            for( int paramId = 0; paramId < 5; paramId++ ) {
                if( !s->status.modules[uiStatus.selectedModule].params[paramId].visible ) continue;

                float value = s->status.modules[uiStatus.selectedModule].params[paramId].value;
                float min = s->status.modules[uiStatus.selectedModule].params[paramId].min;
                float max = s->status.modules[uiStatus.selectedModule].params[paramId].max;
                float increment = uiStatus.paramIncrements[paramId];

                s->status.modules[uiStatus.selectedModule].params[paramId].value = fmax(min, fmin(value + increment, max));
            }
        }

    // ENCODERS
        for( int paramId = 0; paramId < 5; paramId++ ) {

            s->status.encoders[paramId].pressed = s->midi_encoders[paramId].pressed(s->time.engine_frame());

            if( s->midi_encoders[paramId].increment(s->time.engine_frame()) != 0 ) {

                float value = s->status.modules[uiStatus.selectedModule].params[paramId].value;
                float min = s->status.modules[uiStatus.selectedModule].params[paramId].min;
                float max = s->status.modules[uiStatus.selectedModule].params[paramId].max;
                float increment = s->midi_encoders[paramId].increment(s->time.engine_frame());
                increment *= s->status.modules[s->status.selectedModule].params[paramId].step;
                increment *= ((int)s->midi_encoders[paramId].pressed(s->time.engine_frame()) * (MIDI_PUSHED_FACTOR - 1)) + 1;

                s->status.modules[uiStatus.selectedModule].params[paramId].value = fmax(min, fmin(value + increment, max));
                s->midi_encoders[paramId].setIncrement(0, s->time.engine_frame());
            }
        }
    }

    // STATUS -> MODULES
    moduleId = 0;
    for( ModuleStatus status : s->status.modules ) {
        if( !s->status.modules[moduleId].empty )
            s->audioModules[moduleId]->update(status);
        moduleId++;
    }

    // NOTE ON
    for( int note = 0; note < 128; note++ ) {
        if( s->midi_note_on[36] ) {
            s->midi_note_on[36] = false;

            masterId = s->audioModules.size() - 2; // hacky gate the kick
            s->audioModules[masterId]->gate(s->time.engine_frame());
        }
    }

    // PROCESS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->audioModules ) {
        inputId = s->audioWires[moduleId];

        if( inputId == -1 ) {  // Hardware Input
            module->process(ioIn, s->time.engine_frame());
        }
        else if( inputId >= 0 ) {  // Module Input
           module->process(s->audioModules[inputId]->output(), s->time.engine_frame());
        }

        moduleId++;
    }

    masterId = s->audioModules.size() - 1;
    for( nFrame i = 0; i < bufferSize * 2; i++ ) {
        ioOut[i] = s->audioModules[masterId]->output()[i];
    }

    // RECORDER
    s->recorder->write(ioOut);

    // MODULES -> STATUS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : s->audioModules ) {
        s->status.modules[moduleId] = module->status();
        moduleId++;
    }

    // STATUS -> UI
    s->status.clock.bar = s->time.bar();
    s->status.clock.seconds = s->time.seconds();
    s->status.clock.sequence_step = s->time.sequence_step();
    s->status.clock.is_playing = s->time.is_playing();

    s->uiSetStatus(s->uiPtr, s->status);

    // INCREMENT TIME
    s->time.increment_frame(bufferSize);

    return 0;
}

void Engine::_midiCallback(double /*deltaTime*/, std::vector<unsigned char> *message, void *userData)
{
    int encoder = 0;
    Shared* shared = (Shared*)userData;

    if( !shared->ready ) return;

    // DEBUG COUT
//    unsigned int nBytes = message->size();
//    for ( unsigned int i=0; i<nBytes; i++ )
//      std::cout << "Byte " << i << " = " << (int)message->at(i) << ", " << std::endl;

    // TIMING
    if( message->at(0) == 250 ) shared->time.start();
    if( message->at(0) == 252 ) shared->time.stop();
    if( message->at(0) == 251 ) shared->time.resume();

    if( message->at(0) == 248 && shared->time.is_playing() ) { // Midi Clock Pulse
        shared->time.increment_ppqn(1);
    }

    // NOTE ON
    if( message->at(0) >= 144 && message->at(0) <= 159) {
        //std::cout << "Note on : " << (int)message->at(1) << " vel: " << (int)message->at(2) << " chan: " << (int)message->at(0) - 143 << std::endl;
        if( (int)message->at(2) )
            shared->midi_note_on[(int)message->at(1)] = true;
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
                shared->midi_encoders[encoder].setPressed((bool)((int)message->at(2) / 127), shared->time.engine_frame());
            break;

            // NRPN MSB
            case 99:
                shared->midi_msb = (int)message->at(2);
            break;

            // NRPN LSB
            case 98:
                shared->midi_lsb = (int)message->at(2);
            break;

            // DECREASE
            case 97:
                encoder = (shared->midi_lsb << 7) | shared->midi_msb;
                shared->midi_encoders[encoder].setIncrement(-1, shared->time.engine_frame());

                shared->midi_msb = -1;
                shared->midi_lsb = -1;
            break;

            // INCREASE
            case 96:
                encoder = (shared->midi_lsb << 7) | shared->midi_msb;
                shared->midi_encoders[encoder].setIncrement(1, shared->time.engine_frame());

                shared->midi_msb = -1;
                shared->midi_lsb = -1;
            break;

            default:
            break;
        }
    }
}
