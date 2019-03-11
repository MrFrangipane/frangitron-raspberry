#ifndef ENGINE_H
#define ENGINE_H

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>
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
#include "audio/recorder.h"
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"
#include "audio/filter.h"
#include "audio/compressor.h"
#include "audio/kicksynth.h"
#include "audio/sampleplayer.h"


typedef UiStatus(*GetStatusCallback)(void* /*uiPtr*/);
typedef void(*SetStatusCallback)(void* /*uiPtr*/, EngineStatus /*status*/);


struct RegisteredNote {
    RegisteredNote(const int note, const int module) :
        noteNumber(note),
        moduleIndex(module) { }

    const int noteNumber;
    const int moduleIndex;
};


struct Shared {
    MasterClock time;
    std::vector<std::shared_ptr<AbstractModule>> audioModules;
    EngineStatus status;
    std::vector<int> audioWires;
    std::vector<RegisteredNote> registeredNotes;
    void* uiPtr;
    GetStatusCallback uiGetStatus;
    SetStatusCallback uiSetStatus;
    int midi_msb = -1;
    int midi_lsb = -1;
    Encoder midi_encoders[MIDI_ENCODER_COUNT];
    nFrame uiPreviousFrame = 0;
    bool midi_note_on[MIDI_NOTE_COUNT];
    SampleBank * sampleBank = nullptr;
    Recorder* recorder = nullptr;
};


class Engine
{
public:
    Engine(const Configuration *configuration = nullptr);
    void start();
    void stop();
    void setUiCallbacks(void * uiPtr, GetStatusCallback getCallback, SetStatusCallback setCallback) {
        _shared.uiPtr = uiPtr; _shared.uiGetStatus = getCallback; _shared.uiSetStatus = setCallback;
    }
private:
    Shared _shared;
    const Configuration *_configuration;
    RtAudio* _audio = nullptr;
    RtMidiIn* _midi = nullptr;
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
    static void _midiCallback(
        double deltaTime,
        std::vector<unsigned char> *message,
        void *userData
    );
    unsigned int _midiDeviceIndex = 0;
    unsigned int _audioDeviceIndex = 0;
    unsigned int _bufferSize = BUFFER_SIZE;
};

#endif // ENGINE_H
