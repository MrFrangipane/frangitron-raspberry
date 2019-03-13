#ifndef SHARED_H
#define SHARED_H

#include <string>
#include "shared/typedefs.h"
#include "shared/constants.h"

// SHARED
struct EncoderStatus {
    bool pressed = false;
};

struct ModuleParameter {
    std::string name = "";
    float value = 0.0;
    float min = 0.0;
    float max = 1.0;
    float step = 0.0078; // 1.0 / 128
    bool isVisible = false;
};

struct ModuleStatus {
    bool isDummy = false; // This is to avoid to set it to false on every Module->status() call
    ModuleParameter params[MODULE_PARAM_COUNT];
    float levelOut = -60.0;
    bool isClipping = false;
};

struct ClockStatus {
    bool isPlaying = false;
    double seconds = 0.0;
    double bar = 0;
    nSequenceStep sequenceStep = 0;
};

// ENGINE
struct EngineStatus {
    enum Status {
        IDLE = 0,
        LOADING,
        RUNNING,
        STOPPED,
        nActivities
    };
    int loadingProgress = 0;
    Status status = IDLE;
    ModuleStatus modulesStatuses[MODULE_MAX_COUNT];
    int selectedModule = -1;
    EncoderStatus encoders[MIDI_ENCODER_COUNT];
    ClockStatus clock;
};

// UI
struct UiStatus {
    float paramIncrements[MIDI_ENCODER_COUNT] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int selectedModule = -1;
    nFrame frame = 0;
    bool isRunning = true;
};

#endif // SHARED_H

