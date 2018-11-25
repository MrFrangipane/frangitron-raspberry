#ifndef AUDIOMIDI_H
#define AUDIOMIDI_H

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "../include/rtaudio/RtAudio.h"
#include "typedefs.h"
#include "levelmeter.h"
#include "effects/filter.h"
#include "effects/compressor.h"


struct AudioMidiStatus {
    LevelMeterStatus meterInput;
    LevelMeterStatus meterOutput;
    FilterStatus filterInput;
    CompressorStatus compInput;
};


struct Shared {
    nFrame time = 0;
    LevelMeter meterInput;
    LevelMeter meterOutput;
    Filter filterInput;
    Compressor compInput;
    AudioMidiStatus status;
};


class AudioMidi
{
public:
    AudioMidi() {}
    void start();
    AudioMidiStatus status() { return _shared.status; }
    void update(AudioMidiStatus status_) { _shared.status = status_; }
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
