#include "audiomidi.h"

AudioMidi::~AudioMidi() {
    if( _shared.inBuffer ) delete[] _shared.inBuffer;
    if( _shared.outBuffer ) delete[] _shared.outBuffer;
}

void AudioMidi::start()
{
    int deviceIndex = 2;
    unsigned int bufferSize = 64;
    _shared.trackInput = Track(bufferSize);
    _shared.inBuffer = new Sample[bufferSize];
    _shared.outBuffer = new Sample[bufferSize];

    // HACKY POTTER ---------------
    std::string user = std::getenv("USER");
    std::string frangi ("frangi");
    if( user == frangi ) {
        std::cout << "User is Frangi" << std::endl;
        deviceIndex = 3;
        bufferSize = 30;
    } // --------------------------

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
    _audioInParams.deviceId = deviceIndex;
    _audioInParams.nChannels = 2;

    // AUDIO OUT
    RtAudio::StreamParameters _audioOutParams;
    _audioOutParams.deviceId = deviceIndex;
    _audioOutParams.nChannels = 2;

    try
    {
        std::cout << "AudioMidi : Opening audio device and setting callback " <<
                   "(" << deviceIndex << ") " <<
                   _audio->getDeviceInfo(deviceIndex).name <<
                   std::endl;

        RtAudio::StreamOptions options;
        options.flags |= RTAUDIO_SCHEDULE_REALTIME;
        //options.flags |= RTAUDIO_MINIMIZE_LATENCY;

        _audio->openStream(
          &_audioOutParams,
          &_audioInParams,
          RTAUDIO_FLOAT32, // PiSound supports only up to 32 bits
          SAMPLE_RATE,
          &bufferSize,
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
    if( status ) std::cout << "!";

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
