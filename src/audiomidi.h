#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "../include/rtaudio/RtAudio.h"
#include "typedefs.h"
#include "track.h"


struct Status {
    float compIn = 1.0;
    float rmsInL = 0.0;
    float rmsInR = 0.0;
    float rmsOutL = 0.0;
    float rmsOutR = 0.0;
};


struct Shared {
    nFrame time = 0;
    Track trackInput;
    Status status;
};


class AudioMidi
{
public:
    AudioMidi() {}
    void start();
    Status status() { return _shared.status; }
private:
    RtAudio* _audio = NULL;
    void _setAudioDeviceIndex();
    static int _audioCallback(
        void* bufferOut,
        void* bufferIn,
        unsigned int bufferSize,
        double streamTime,
        RtAudioStreamStatus status,
        void *userData
    );
    unsigned int _deviceIndex;
    unsigned int _bufferSize = 128;
    Shared _shared;
};

#endif // AUDIOMIDI_H
