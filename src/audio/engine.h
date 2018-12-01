#ifndef ENGINE_H
#define ENGINE_H

#include <atomic>
#include <vector>
#include <memory>
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
    std::vector<Status> moduleStatuses;
};


struct EngineShared {
    nFrame time = 0;
    std::vector<std::shared_ptr<AbstractModule>> modules;
    std::atomic_bool is_updating;
    EngineStatus status;
};


class Engine
{
public:
    Engine() { _shared.is_updating.store(false); }
    void start();
    EngineStatus status();
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
