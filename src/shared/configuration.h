#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <string>
#include "shared/constants.h"


struct ModuleConfigOverride {
    bool active = false;
    float value = 0;
};


struct ModuleLayout {
    int row = 0;
    int col = 0;
    int rowSpan = 1;
    int colSpan = 1;
    int rowStretch = -1;
    int colStretch = -1;
};


struct ModuleConfig
{
    std::string name = "Module";
    std::string type = "unknown";
    int wireIndex = -1;
    ModuleConfigOverride overrides[MODULE_PARAM_COUNT];
    ModuleLayout layout;
};


struct Configuration
{
    ModuleConfig modules[MODULE_MAX_COUNT];
};

#endif // CONFIGURATION_H
