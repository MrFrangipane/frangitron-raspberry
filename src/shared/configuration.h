#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <string>
#include "shared/typedefs.h"
#include "shared/constants.h"


struct ConfModuleParamOverride {
    bool active = false;
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
    int wireIndex = -1;
    int midiNote = -1;
    ConfModuleParamOverride overrides[MODULE_PARAM_COUNT];
    ConfModuleLayout layout;
};


struct ConfAudioClip
{
    std::string name = "unknown";
    std::string filepath = "";
    int channelCount = 1;
    nFrame frameCount = 0;
};


struct Configuration
{
    ConfModule modules[MODULE_MAX_COUNT];
    ConfAudioClip audioClips[AUDIOCLIP_MAX_COUNT];
};


void initModules(Configuration* conf);
void initSamples(Configuration* conf);
void initConfig(Configuration* conf);


#endif // CONFIGURATION_H
