#ifndef SHARED_H
#define SHARED_H

#include <string>
#include "shared/typedefs.h"
#include "shared/constants.h"


// AUDIO FILES
struct AudioFileMarkerInfos
{
    std::string name = "unknown";
    nFrame position = -1;
};


struct AudioFileInfos
{
    std::string name = "unknown";
    std::string filepath = "";
    int channelCount = 0;
    nFrame frameCount = 0;
    AudioFileMarkerInfos markers[AUDIO_MARKER_COUNT_MAX];
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
    bool isVisible = false;
};

struct ModuleStatus {
    bool isDummy = false; // This is to avoid to set it to false on every Module->status() call
    ModuleParameter params[MODULE_PARAM_COUNT];
    float levelOut = -60.0;
    bool isClipping = false;

};

// UI
struct UiStatus {
    float paramIncrements[MIDI_ENCODER_COUNT] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int selectedModule = -1;
    nFrame frame = 0;
    bool isRunning = true;
};

#endif // SHARED_H

