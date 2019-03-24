#ifndef ENGINE_H
#define ENGINE_H

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <thread>
#include <chrono>
#include <thread>
#include "sndfile.hh"
#include "../include/rtaudio/RtAudio.h"
#include "../include/rtmidi/RtMidi.h"
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/structures.h"
#include "shared/configuration.h"
#include "shared/masterclock.h"
#include "midi/encoder.h"
#include "audio/_samplebank.h"
#include "audio/_djtrackbank.h"
#include "audio/recorder.h"
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"
#include "audio/filter.h"
#include "audio/compressor.h"
#include "audio/kicksynth.h"
#include "audio/sampleplayer.h"
#include "audio/djdeck.h"


// ENGINE
struct EngineStatus {
    enum Status {
        IDLE = 0,
        LOADING,
        RUNNING,
        STOPPED,
        AUDIO_ERROR,
        MIDI_ERROR,
        nActivities
    };
    int loadingProgress = 0;
    Status status = IDLE;
    ModuleStatus modulesStatuses[MODULE_MAX_COUNT];
    int selectedModule = -1;
    EncoderStatus encoders[MIDI_ENCODER_COUNT];
    ClockStatus clock;
    SampleBank * sampleBank = nullptr;
    DjTrackBank * trackBank = nullptr;
    Buffer emptyBuffer;
};


typedef UiStatus(*GetStatusCallback)(void* /*uiPtr*/);
typedef void(*SetStatusCallback)(void* /*uiPtr*/, EngineStatus /*status*/);


struct SubscribedNote {
    SubscribedNote(const int note, const int module) :
        noteNumber(note),
        moduleIndex(module) { }

    const int noteNumber;
    const int moduleIndex;
};


struct Shared {
    MasterClock time;
    std::vector<std::shared_ptr<AbstractModule>> patch;
    EngineStatus engine;
    std::vector<int> patchWires;
    std::vector<SubscribedNote> subscribedNotes;
    void* uiPtr;
    GetStatusCallback uiStatus;
    SetStatusCallback uiSetStatus;
    int midiMsb = -1;
    int midiLsb = -1;
    Encoder midiEncoders[MIDI_ENCODER_COUNT] = {};
    nFrame uiFrame = 0;
    bool midiNoteOn[MIDI_NOTE_COUNT] = {};
    Recorder * recorder = nullptr;
    int midiPortNumber = 0;
};


class Engine
{
public:
    Engine(const Configuration * configuration = nullptr);
    void start();
    void stop();
    void setUiCallbacks(void * uiPtr, GetStatusCallback getCallback, SetStatusCallback setCallback) {
        _shared.uiPtr = uiPtr; _shared.uiStatus = getCallback; _shared.uiSetStatus = setCallback;
    }
private:
    Shared _shared;
    const Configuration *_configuration;
    RtAudio* _audio = nullptr;
    RtMidiIn* _midiIn = nullptr;
    Recorder* _recorder = nullptr;
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
    static void _midiInCallback(
        double deltaTime,
        std::vector<unsigned char> *message,
        void *userData
    );
    static void _midiOutLoop(Shared * shared);
    unsigned int _midiDeviceIndex = 0;
    unsigned int _audioDeviceIndex = 0;
    unsigned int _bufferSize = BUFFER_SIZE;
    std::thread _midiOutThread;
};

#endif // ENGINE_H
