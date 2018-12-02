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
        std::cout << "AudioMidi : Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    std::cout << std::endl << "Available Devices :" << std::endl;

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
    // HACKY POTTER (to work with Fireface UCX) ---
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        _bufferSize = 60;
    } // ------------------------------------------

    // MODULES
    _shared.modules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.moduleStatuses.push_back(_shared.modules[0]->status());
    _shared.wires.push_back(-1);  // Input

    _shared.modules.push_back(std::make_shared<Filter>(Filter(_bufferSize)));
    _shared.status.moduleStatuses.push_back(_shared.modules[1]->status());
    _shared.wires.push_back(0);  // Level Meter

    _shared.modules.push_back(std::make_shared<Compressor>(Compressor(_bufferSize)));
    _shared.status.moduleStatuses.push_back(_shared.modules[2]->status());
    _shared.wires.push_back(1);  // Filter

    _shared.modules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.moduleStatuses.push_back(_shared.modules[3]->status());
    _shared.wires.push_back(2);  // Compressor

    // AUDIO DEVICE
    _setAudioDeviceIndex();
    try
    {
        _audio = new RtAudio();
    }
    catch (RtAudioError &error)
    {
        std::cout << "Error while allocating Audio" << std::endl;
        std::cout << error.getMessage() << std::endl;
        return;
    }

    RtAudio::StreamParameters _audioInParams;
    _audioInParams.deviceId = _deviceIndex;
    _audioInParams.nChannels = 2;

    RtAudio::StreamParameters _audioOutParams;
    _audioOutParams.deviceId = _deviceIndex;
    _audioOutParams.nChannels = 2;

    try
    {
        std::cout << "Opening audio device " <<
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
          std::cout << "Error while opening audio device and setting callback" << std::endl;
          std::cout << error.getMessage() << std::endl;
      }
}

EngineStatus Engine::status() const
{
    while( _shared.isWritingStatus ) { }
    return _shared.status;
}

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    // INIT
    int moduleId = 0;
    int inputId = -2;
    int masterId = 0;
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    EngineShared* shared = (EngineShared*)userData;

    // UI -> STATUS
    shared->isWritingStatus = true;
    moduleId = 0;
    EngineStatus uiEngineStatus = shared->uiEngineStatusCallback(shared->uiPtr);
    for( Status status : uiEngineStatus.moduleStatuses ) {
        shared->status.moduleStatuses[moduleId] = status;
        moduleId++;
    }
    shared->isWritingStatus = false;

    // STATUS -> MODULES
    moduleId = 0;
    for( Status status : shared->status.moduleStatuses ) {
        if( !status.empty() )
            shared->modules[moduleId]->update(status);
        moduleId++;
    }

    // PROCESS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : shared->modules ) {
        inputId = shared->wires[moduleId];

        if( inputId == -1 ) {  // Audio Input
            module->process(ioIn, shared->time);
        }
        else if( inputId >= 0 ) {  // Other modules
           module->process(shared->modules[inputId]->output(), shared->time);
        }

        moduleId++;
    }

    masterId = shared->modules.size() - 1;
    for( nFrame i = 0; i < bufferSize; i++ ) {
        ioOut[i * 2] = shared->modules[masterId]->output()[i * 2];
        ioOut[i * 2 + 1] = shared->modules[masterId]->output()[i * 2 + 1];
    }

    // MODULES -> STATUS
    shared->isWritingStatus = true;
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : shared->modules ) {
        shared->status.moduleStatuses[moduleId] = module->status();
        moduleId++;
    }
    shared->isWritingStatus = false;

    // INCREMENT TIME
    shared->time += bufferSize;

    return 0;
}
