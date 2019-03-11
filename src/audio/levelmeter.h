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
    LevelMeter(const nFrame bufferSize = 0) : AbstractModule(bufferSize) { }

    ModuleStatus const status() override;
    void update(ModuleStatus status);
    void process(Sample const * bufferIn, const nFrame /*time*/, const SampleBank * /*sampleBank*/) override;
private:
    float _level = 0.0;
    bool _is_level_locked = false;
    _SampleMeter _meterL;
    _SampleMeter _meterR;
    bool _is_clipping = false;
    int _clip_release = 0; // count buffers to persist clipping
};

#endif // LEVELMETER_H
