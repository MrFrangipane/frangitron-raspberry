#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include "../include/rtaudio/RtAudio.h"
#include "track.h"

class AudioMidi
{
public:
    AudioMidi();
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
};

#endif // AUDIOMIDI_H
