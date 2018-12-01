#include "engine.h"

void Engine::_setAudioDeviceIndex()
{
    std::vector<std::string> interfaceNames;
    interfaceNames.push_back("pisound");
    interfaceNames.push_back("Fireface");

    // HACKY POTTER (to work with Fireface UCX) ---
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        _bufferSize = 30;
    } // ------------------------------------------

    RtAudio* audio;

    try {
        audio = new RtAudio();
    }
    catch (RtAudioError &error) {
        std::cout << "AudioMidi : Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    std::cout << std::endl << "AudioMidi : Available Devices" << std::endl;

    RtAudio::DeviceInfo device_infos;

    for (unsigned int i = 0; i < audio->getDeviceCount(); i++)
    {
        device_infos = audio->getDeviceInfo(i);
        std::cout << i << " : " << device_infos.name <<
                    " duplex:" << device_infos.duplexChannels <<
                    " in:" << device_infos.inputChannels <<
                    " out:" << device_infos.outputChannels <<
                     std::endl;
        for (auto interfaceName = interfaceNames.begin(); interfaceName != interfaceNames.end(); interfaceName++)
        {
            if (device_infos.inputChannels != 0 &&
                device_infos.outputChannels != 0 &&
                device_infos.name.find(*interfaceName) != std::string::npos)
            {
                _deviceIndex = i;
                return;
            }
        }
    }

    _deviceIndex = 0;
}

void Engine::start()
{
    // AUDIO DEVICES
    _setAudioDeviceIndex();

    // SHARED
    _shared.modules.push_back(std::make_shared<LevelMeter>(LevelMeter()));
    _shared.modules.push_back(std::make_shared<Filter>(Filter(_bufferSize)));
    _shared.modules.push_back(std::make_shared<Compressor>(Compressor(_bufferSize)));
    _shared.modules.push_back(std::make_shared<LevelMeter>(LevelMeter()));

    _shared.status.moduleStatuses.push_back(Status());
    _shared.status.moduleStatuses.push_back(Status());
    _shared.status.moduleStatuses.push_back(Status());
    _shared.status.moduleStatuses.push_back(Status());

    // HACKY POTTER (Until Midi is back) ---
    // Filter at 0
    Status s = _shared.modules[1]->status();
    s["cutoff"] = 0;
    _shared.modules[1]->update(s);

    // Comp at -20.0
    s = _shared.modules[2]->status();
    s["threshold"] = -20.0;
    _shared.modules[2]->update(s);
    // -------------------------------------

    // NEW DAC
    try
    {
        _audio = new RtAudio();
    }
    catch (RtAudioError &error)
    {
        std::cout << "AudioMidi : Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    // AUDIO IN
    RtAudio::StreamParameters _audioInParams;
    _audioInParams.deviceId = _deviceIndex;
    _audioInParams.nChannels = 2;

    // AUDIO OUT
    RtAudio::StreamParameters _audioOutParams;
    _audioOutParams.deviceId = _deviceIndex;
    _audioOutParams.nChannels = 2;

    try
    {
        std::cout << "AudioMidi : Opening audio device " <<
                     "(" << _deviceIndex << ") " <<
                     "and setting callback : " <<
                   _audio->getDeviceInfo(_deviceIndex).name <<
                   std::endl;

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
      catch (RtAudioError &error)
      {
          std::cout << "AudioMidi : Error while opening audio device and setting callback" << std::endl;
          std::cout << error.getMessage() << std::endl;
      }
}

EngineStatus Engine::status()
{
    while( _shared.is_updating.load() ) { }
    return _shared.status;
}

void Engine::update(EngineStatus status_)
{
    while( _shared.is_updating.load() ) { }
    _shared.status = status_;
}

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    // INIT
    int i;
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    EngineShared* shared = (EngineShared*)userData;

    // STATUS -> MODULES
    i = 0;
    shared->is_updating.store(true);
    for( Status status : shared->status.moduleStatuses ) {
        if( !status.empty() )
            shared->modules[i]->update(status);
        i++;
    }
    shared->is_updating.store(false);

    // PROCESS
    shared->modules[1]->process(ioIn, shared->time);
    shared->modules[2]->process(shared->modules[1]->output(), shared->time);

    for( nFrame i = 0; i < bufferSize; i++ ) {
        std::dynamic_pointer_cast<LevelMeter>(shared->modules[0])->stepComputations(ioIn[i * 2], ioIn[i * 2 + 1]);

        ioOut[i * 2] = shared->modules[2]->output()[i * 2];
        ioOut[i * 2 + 1] = shared->modules[2]->output()[i * 2 + 1];

        std::dynamic_pointer_cast<LevelMeter>(shared->modules[3])->stepComputations(ioOut[i * 2], ioOut[i * 2 + 1]);
    }

    // MODULES -> STATUS
    shared->is_updating.store(true);
    i = 0;
    for( std::shared_ptr<AbstractModule> module : shared->modules ) {
        shared->status.moduleStatuses[i] = module->status();
        i++;
    }
    shared->is_updating.store(false);

    // INCREMENT TIME
    shared->time += bufferSize;

    return 0;
}
