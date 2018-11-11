#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include "../include/rtaudio/RtAudio.h"
#include "typedefs.h"
#include "track.h"


struct Shared {
    Track track;
    Sample* in;
    Sample* out;
};


class AudioMidi
{
public:
    AudioMidi();
    AudioMidi(int buffer_size) : _buffer_size(buffer_size) {}
    void start();
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
    int _buffer_size;
};

#endif // AUDIOMIDI_H
