#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include "shared/typedefs.h"
#include "shared/constants.h"
#include "shared/structures.h"


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
    bool routedToMaster = false;
};


struct Configuration
{
    ConfModule modules[MODULE_MAX_COUNT];
    AudioFileInfos samples[AUDIO_SAMPLE_MAX_COUNT];  // pre-loaded
    AudioFileInfos djTracks[AUDIO_DJTRACK_MAX_COUNT];  // async-loaded
};


void initPatch(Configuration * conf);
void initSamples(Configuration * conf);
void initDjTracks(Configuration * conf);
void initConfig(Configuration * conf);


#endif // CONFIGURATION_H
