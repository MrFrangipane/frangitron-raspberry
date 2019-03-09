#ifndef SHARED_H
#define SHARED_H

#include <string>
#include "shared/typedefs.h"

// CONFIGURATION
struct Configuration {
    std::string wav_files[128];
};

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
    bool visible = false;
};

struct ModuleStatus {
    bool empty = true;
    ModuleParameter params[16];
    float levelOut;
    bool is_clipping;
};

struct ClockStatus {
    bool is_playing = false;
    double seconds = 0.0;
    double bar = 0;
    nSequenceStep sequence_step = 0;
};

// ENGINE
struct EngineStatus {
    ModuleStatus modules[16];
    int selectedModule = -1;
    EncoderStatus encoders[5];
    ClockStatus clock;
};

// UI
struct UiStatus {
    float paramIncrements[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int selectedModule = -1;
    nFrame frame = 0;
    bool running = true;
};

#endif // SHARED_H

