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


typedef EngineStatus(*GetStatusCallback)(void* /*uiPtr*/);
typedef void(*SetStatusCallback)(void* /*uiPtr*/, EngineStatus /*status*/);


struct EngineShared {
    nFrame time = 0;
    std::vector<std::shared_ptr<AbstractModule>> modules;
    EngineStatus status;
    std::vector<int> wires;
    GetStatusCallback uiGetStatusCallback;
    SetStatusCallback uiSetStatusCallback;
    void* uiPtr;
    std::atomic<bool> isWritingStatus{false};
    std::atomic<bool> isReadingStatus{false};
};


class Engine
{
public:
    Engine() { }
    void start();
    void setStatusCallbacks(void * uiPtr, GetStatusCallback getCallback, SetStatusCallback setCallback) {
        _shared.uiPtr = uiPtr; _shared.uiGetStatusCallback = getCallback; _shared.uiSetStatusCallback = setCallback;
    }
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
