#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../include/rtaudio/RtAudio.h"
#include "typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"
#include "audio/filter.h"
#include "audio/compressor.h"


struct EngineStatus {
    std::vector<Status> _statuses;
};


struct EngineShared {
    nFrame time = 0;
    std::vector<AbstractModule> modules;
    EngineStatus status;
};


class Engine
{
public:
    Engine() {}
    void start();
    EngineStatus status() { return _shared.status; }
    void update(EngineStatus status_) { _shared.status = status_; }
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
    EngineShared _shared;
};

#endif // ENGINE_H
