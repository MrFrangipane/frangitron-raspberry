#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../include/rtaudio/RtAudio.h"
#include "typedefs.h"
#include "track.h"


struct Status {
    float rmsInL = 0.0;
    float rmsInR = 0.0;
    float rmsOutL = 0.0;
    float rmsOutR = 0.0;
};


struct Shared {
    nFrame time = 0;
    Track track_input;
    Sample* in;
    Sample* out;
    Status status;
};


class AudioMidi
{
public:
    AudioMidi() {}
    ~AudioMidi();
    void start();
    Status status() { return _shared.status; }
private:
    RtAudio* _audio;
    static int _audioCallback(
        void* bufferOut,
        void* bufferIn,
        unsigned int bufferSize,
        double streamTime,
        RtAudioStreamStatus status,
        void *userData
    );
    Shared _shared;
};

#endif // AUDIOMIDI_H
