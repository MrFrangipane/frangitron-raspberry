#ifndef LEVELMETER_H
#define LEVELMETER_H

#include "cmath"
#include "shared/typedefs.h"
#include "audio/_samplemeter.h"
#include "audio/abstractmodule.h"


class LevelMeter : public AbstractModule
{
public:
    LevelMeter(const nFrame bufferSize = 0) : AbstractModule(bufferSize) { }

    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    _SampleMeter _meterL;
    _SampleMeter _meterR;
    Sample _clip_tresh = 1.0;
    bool _is_clipping = false;
    int _clip_release = 0; // count buffers to persist clipping
};

#endif // LEVELMETER_H
