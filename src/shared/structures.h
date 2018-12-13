#ifndef SHARED_H
#define SHARED_H

#include <string>

// SHARED
struct EncoderStatus {
    bool isPressed = false;
    int increment = 0;
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
    ModuleParameter params[16];
    float levelOut;
};

// ENGINE
struct EngineStatus {
    EncoderStatus encoders[5];
    ModuleStatus modules[16];
    int selectedModule = -1;
};

// UI
struct UiStatus {
    float sliderPosition[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int selectedModule = -1;
};

// MIDI
struct MidiStatus {
    EncoderStatus encoders[5];
};

#endif // SHARED_H

