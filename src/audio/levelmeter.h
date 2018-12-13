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
    void update(ModuleStatus /*status_*/) override { }
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    _SampleMeter _meterL;
    _SampleMeter _meterR;
};

#endif // LEVELMETER_H
