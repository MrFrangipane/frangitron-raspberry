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
    TrackStatus input;
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
    RtAudio* _audio = nullptr;
    void _setAudioDeviceIndex();
    static int _audioCallback(
        void* bufferOut,
        void* bufferIn,
        unsigned int bufferSize,
        double streamTime,
        RtAudioStreamStatus status,
        void *userData
    );
    unsigned int _deviceIndex = 0;
    unsigned int _bufferSize = 128;
    Shared _shared;
};

#endif // AUDIOMIDI_H
