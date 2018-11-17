#include "audiomidi.h"

void AudioMidi::_setAudioDeviceIndex()
{
    std::vector<std::string> interfaceNames;
    interfaceNames.push_back("pisound");
    interfaceNames.push_back("Fireface");

    // HACKY POTTER ---------------
    if( std::string(std::getenv("USER")) == std::string("frangi") ) {
        _deviceIndex = 3;
        _bufferSize = 30;
    } // --------------------------

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
        for (std::vector<std::string>::iterator interfaceName = interfaceNames.begin(); interfaceName != interfaceNames.end(); interfaceName++)
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

void AudioMidi::start()
{
    _setAudioDeviceIndex();
    _shared.trackInput = Track(_bufferSize);

    std::cout << std::endl;

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
        std::cout << "AudioMidi : Expected latency : " << _audio->getStreamLatency() << " frames" << std::endl;

        _audio->startStream();
      }
      catch (RtAudioError &error)
      {
          std::cout << "AudioMidi : Error while opening audio device and setting callback" << std::endl;
          std::cout << error.getMessage() << std::endl;
      }
}

int AudioMidi::_audioCallback(void* bufferOut, void* bufferIn, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void* userData)
{
    // CAST
    Sample *ioIn = (Sample*)bufferIn;
    Sample *ioOut = (Sample*)bufferOut;
    Shared* shared = (Shared*)userData;

    // PROCESS
    shared->trackInput.process(ioIn, ioOut, shared->time);

    // UPDATE STATUS
    shared->status.compIn = shared->trackInput.compressor.level;
    shared->status.rmsInL = shared->trackInput.levelMeterIn.rmsL;
    shared->status.rmsInR = shared->trackInput.levelMeterIn.rmsR;
    shared->status.rmsOutL = shared->trackInput.levelMeterOut.rmsL;
    shared->status.rmsOutR = shared->trackInput.levelMeterOut.rmsR;

    // INCREMENT TIME
    shared->time += bufferSize;

    return 0;
}
