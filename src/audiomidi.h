#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include "../include/rtaudio/RtAudio.h"
#include "track.h"

class AudioMidi
{
public:
    AudioMidi();
    AudioMidi(int buffer_size) : _buffer_size(buffer_size) {}
    void start();
private:
    static int _audioCallback(
        void* bufferOut,
        void* bufferIn,
        unsigned int bufferSize,
        double streamTime,
        RtAudioStreamStatus status,
        void *userData
    );
    Track _track;
    int _buffer_size;
};

#endif // AUDIOMIDI_H
