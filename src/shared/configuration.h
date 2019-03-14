#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <string>
#include "shared/typedefs.h"
#include "shared/constants.h"


struct ConfModuleParamOverride {
    bool isActive = false;
    float value = 0;
};


struct ConfModuleLayout {
    int row = 0;
    int col = 0;
    int rowSpan = 1;
    int colSpan = 1;
    int rowStretch = -1;
    int colStretch = -1;
};


struct ConfModule
{
    std::string name = "Module";
    std::string type = "unknown";
    int inputIndex = -1;
    int midiNote = -1;
    ConfModuleParamOverride overrides[MODULE_PARAM_COUNT];
    ConfModuleLayout layout;
};


struct ConfAudioMarker
{
    std::string name = "unknown";
    nFrame position = -1;
};


struct ConfAudioFile
{
    std::string name = "unknown";
    std::string filepath = "";
    int channelCount = 1;
    nFrame frameCount = 0;
    ConfAudioMarker markers[AUDIO_MARKER_COUNT_MAX];
};


struct Configuration
{
    ConfModule modules[MODULE_MAX_COUNT];
    ConfAudioFile samples[AUDIO_SAMPLE_MAX_COUNT];  // pre-loaded
    ConfAudioFile clips[AUDIO_CLIP_MAX_COUNT];  // async-loaded
};


void initModules(Configuration* conf);
void initSamples(Configuration* conf);
void initConfig(Configuration* conf);


#endif // CONFIGURATION_H
