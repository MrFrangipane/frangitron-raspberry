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
    _shared.filterInput = Filter(_bufferSize);
    _shared.compInput = Compressor(_bufferSize);

    // HACKY POTTER (Until Midi is back) ---
    FilterStatus s = _shared.filterInput.status();
    s.cutoff = 0;
    _shared.filterInput.update(s);
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

int Engine::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double /*streamTime*/, RtAudioStreamStatus /*status*/, void* userData)
{
    // CAST
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    EngineShared* shared = (EngineShared*)userData;

    //UPDATE
    shared->compInput.update(shared->status.compInput);
    shared->filterInput.update(shared->status.filterInput);

    // PROCESS
    shared->filterInput.process(ioIn, shared->time);
    shared->compInput.process(shared->filterInput.output(), shared->time);

    for( nFrame i = 0; i < bufferSize; i++ ) {
        shared->meterInput.stepComputations(ioIn[i * 2], ioIn[i * 2 + 1]);

        ioOut[i * 2] = shared->compInput.output()[i * 2];
        ioOut[i * 2 + 1] = shared->compInput.output()[i * 2 + 1];

        shared->meterOutput.stepComputations(ioOut[i * 2], ioOut[i * 2 + 1]);
    }

    // UPDATE STATUS
    shared->status.meterInput = shared->meterInput.status();
    shared->status.filterInput = shared->filterInput.status();
    shared->status.compInput = shared->compInput.status();
    shared->status.meterOutput = shared->meterOutput.status();

    // INCREMENT TIME
    shared->time += bufferSize;

    return 0;
}
