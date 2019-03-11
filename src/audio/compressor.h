#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "shared/typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/_samplemeter.h"

class Compressor : public AbstractModule
{
public:
    Compressor(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize) { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/, const SampleBank * /*sampleBank*/) override;
private:
    nFrame _nCycles = 0;
    nFrame _time = 0;
    nFrame _nLerp = 0;
    float _threshold = 0.0;
    float _attack = .080;
    float _release = .200;
    float _ratio = 1.0;
    float _gain = 1.0;
    float _level = 1.0;
    float _levelPrevious = 1.0;
    float _levelTarget = 1.0;
    float _rmsIn = 0.0;
    bool _gate = false;
    _SampleMeter _inMeterL;
    _SampleMeter _inMeterR;
};

#endif // COMPRESSOR_H
