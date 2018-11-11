#include "audiomidi.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

AudioMidi::AudioMidi() {}

void AudioMidi::start()
{
    int deviceIndex = 2;
    unsigned int bufferSize = 64;
    _shared.track = Track(bufferSize);
    _shared.in = new Sample[bufferSize];
    _shared.out = new Sample[bufferSize];

    // HACKY POTTER ---------------
    std::string user = std::getenv("USER");
    std::string frangi ("frangi");
    if( user == frangi ) {
        std::cout << "User is Frangi" << std::endl;
        deviceIndex = 3;
        bufferSize = 30;
    } else {
        std::cout << "On the raspberry pi !" << std::endl;
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
          RTAUDIO_SINT32,
          48000,
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
    IOSample *ioIn = (IOSample*)bufferIn;
    IOSample *ioOut = (IOSample*)bufferOut;
    Shared* shared = (Shared*)userData;

    // NORMALIZE INT -> FLOAT
    for( int i = 0; i < bufferSize; i++ ) {
        //shared->in[i] = (Sample)ioIn[i] / std::numeric_limits<Sample>::max();
        ioOut[i] = ioIn[i]; // shared->in[i] * std::numeric_limits<Sample>::max();

        //std::cout << ioIn[i] << " " << ioOut[i] << " " << std::numeric_limits<Sample>::max() << std::endl;
    }
    // PROCESS
    //shared->track.process(shared->in, shared->out, 0);

    // NORMALIZE FLOAT -> INT
    /*
    for( int i = 0; i < bufferSize; i++ ) {
        ioOut[i] = (IOSample)(shared->out[i] * INT_FAST32_MAX);
    }
    */

    return 0;
}
