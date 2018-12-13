#ifndef ENGINE_H
#define ENGINE_H

#include <atomic>
#include <vector>
#include <memory>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include "../include/rtaudio/RtAudio.h"
#include "../include/rtmidi/RtMidi.h"
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"
#include "audio/filter.h"
#include "audio/compressor.h"


typedef UiStatus(*GetStatusCallback)(void* /*uiPtr*/);
typedef void(*SetStatusCallback)(void* /*uiPtr*/, EngineStatus /*status*/);


struct EngineShared {
    nFrame time = 0;
    std::vector<std::shared_ptr<AbstractModule>> audioModules;
    EngineStatus status;
    std::vector<int> audioWires;
    void* uiPtr;
    GetStatusCallback uiGetStatus;
    SetStatusCallback uiSetStatus;
};


class Engine
{
public:
    Engine() { }
    void start();
    void setUiCallbacks(void * uiPtr, GetStatusCallback getCallback, SetStatusCallback setCallback) {
        _shared.uiPtr = uiPtr; _shared.uiGetStatus = getCallback; _shared.uiSetStatus = setCallback;
    }
private:
    RtAudio* _audio = nullptr;
    RtMidiIn* _midi = nullptr;
    void _setAudioDeviceIndex();
    void _setMidiDeviceIndex();
    static int _audioCallback(
        void* bufferOut,
        void* bufferIn,
        unsigned int bufferSize,
        double streamTime,
        RtAudioStreamStatus status,
        void *userData
    );
    static void _midiCallback(
        double deltaTime,
        std::vector<unsigned char> *message,
        void *userData
    );
    unsigned int _midiDeviceIndex = 0;
    unsigned int _audioDeviceIndex = 0;
    unsigned int _bufferSize = 128;
    EngineShared _shared;
};

#endif // ENGINE_H
