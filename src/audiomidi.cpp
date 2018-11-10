#include "audiomidi.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

AudioMidi::AudioMidi() {}

void AudioMidi::start()
{
    RtAudio* _audio;
    int deviceIndex = 2;
    unsigned int bufferSize = 32;
    _track = Track(bufferSize);

    // HACKY POTTER ---------------
    std::string user = std::getenv("USER");
    std::string frangi ("frangi");
    if( user == frangi ) {
        std::cout << "User is Frangi" << std::endl;
        deviceIndex = 3;
        bufferSize = 30;
    }
    // ----------------------------

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
          options.flags |= RTAUDIO_MINIMIZE_LATENCY;

          _audio->openStream(
              &_audioOutParams,
              &_audioInParams,
              RTAUDIO_FLOAT64,
              48000,
              &bufferSize,
              &_audioCallback,
              &_track,
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
    Sample* in = (Sample*)bufferIn;
    Sample* out = (Sample*)bufferOut;
    Track* track = (Track*)userData;

    //memcpy(bufferOut, bufferIn, bufferSize * 2 * 16);
    track->process(in, out, 0);

    return 0;
}
