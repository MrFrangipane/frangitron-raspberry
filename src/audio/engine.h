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


typedef EngineStatus(*EngineStatusCallback)(void* /*calleePtr*/);


struct EngineShared {
    nFrame time = 0;
    std::vector<std::shared_ptr<AbstractModule>> modules;
    std::atomic_bool isUpdating;
    EngineStatus status;
    std::vector<int> wires;
    EngineStatusCallback uiEngineStatusCallback;
    void* uiPtr;
};


class Engine
{
public:
    Engine() { _shared.isUpdating.store(false); }
    void start();
    EngineStatus status() { return _shared.status; }
    void setStatusCallback(void * uiPtr, EngineStatusCallback callback) { _shared.uiPtr = uiPtr; _shared.uiEngineStatusCallback = callback; }
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
