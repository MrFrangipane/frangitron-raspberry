#ifndef LEVELMETER_H
#define LEVELMETER_H

#include "cmath"
#include "shared/typedefs.h"
#include "shared/constants.h"
#include "shared/audiohelpers.h"
#include "audio/_samplemeter.h"
#include "audio/abstractmodule.h"


class LevelMeter : public AbstractModule
{
public:
    LevelMeter(const nFrame bufferSize = 0, const bool isRoutedToMaster = false) :
        AbstractModule(bufferSize, isRoutedToMaster) { }

    ModuleStatus const status() override;
    void update(ModuleStatus status);
    void process(Sample const * bufferIn, const ClockStatus /*time*/) override;
private:
    float _level = 0.0;
    bool _isLevelLocked = false;
    _SampleMeter _meterL;
    _SampleMeter _meterR;
    bool _isClipping = false;
    int _clippingPersistCount = 0; // count buffers to persist clipping
};

#endif // LEVELMETER_H
