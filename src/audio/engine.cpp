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
    _shared.audioModules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.modules[0] = _shared.audioModules[0]->status();
    _shared.audioWires.push_back(-1);  // Input

    _shared.audioModules.push_back(std::make_shared<Filter>(Filter(_bufferSize)));
    _shared.status.modules[1] = _shared.audioModules[1]->status();
    _shared.audioWires.push_back(0);  // Level Meter

    _shared.audioModules.push_back(std::make_shared<Compressor>(Compressor(_bufferSize)));
    _shared.status.modules[2] = _shared.audioModules[2]->status();
    _shared.audioWires.push_back(1);  // Filter

    _shared.audioModules.push_back(std::make_shared<LevelMeter>(LevelMeter(_bufferSize)));
    _shared.status.modules[3] = _shared.audioModules[3]->status();
    _shared.audioWires.push_back(2);  // Compressor

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

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    // INIT
    int moduleId = 0;
    int inputId = -2;
    int masterId = 0;
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    EngineShared* shared = (EngineShared*)userData;

    // UI -> STATUS (all)
    EngineStatus engineStatus = shared->uiGetStatus(shared->uiPtr);
    if( !engineStatus.modules[0].params[0].name.empty() )
        shared->status = engineStatus;

    // STATUS -> MODULES (parameters)
    moduleId = 0;
    for( ModuleStatus status : shared->status.modules ) {
        if( !status.params[0].name.empty() )
            shared->audioModules[moduleId]->update(status);
        moduleId++;
    }

    // PROCESS
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : shared->audioModules ) {
        inputId = shared->audioWires[moduleId];

        if( inputId == -1 ) {  // Hardware Input
            module->process(ioIn, shared->time);
        }
        else if( inputId >= 0 ) {  // Module Input
           module->process(shared->audioModules[inputId]->output(), shared->time);
        }

        moduleId++;
    }

    masterId = shared->audioModules.size() - 1;
    for( nFrame i = 0; i < bufferSize; i++ ) {
        ioOut[i * 2] = shared->audioModules[masterId]->output()[i * 2];
        ioOut[i * 2 + 1] = shared->audioModules[masterId]->output()[i * 2 + 1];
    }

    // MODULES -> STATUS (all)
    moduleId = 0;
    for( std::shared_ptr<AbstractModule> module : shared->audioModules ) {
        shared->status.modules[moduleId] = module->status();
        moduleId++;
    }

    // STATUS ->UI (all)
    shared->uiSetStatus(shared->uiPtr, shared->status);

    // INCREMENT TIME
    shared->time += bufferSize;

    return 0;
}
