#ifndef SHARED_H
#define SHARED_H

#include <string>
#include "shared/typedefs.h"

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
};

// ENGINE
struct EngineStatus {
    ModuleStatus modules[16];
    int selectedModule = -1;
    EncoderStatus encoders[5];
};

// UI
struct UiStatus {
    float paramIncrements[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int selectedModule = -1;
    nFrame frame = 0;
};

#endif // SHARED_H

