#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "shared/typedefs.h"
#include "shared/structures.h"
#include "shared/masterclock.h"
#include "audio/_samplemeter.h"
#include "audio/_samplebank.h"

class AbstractModule
{
public:
    AbstractModule(const nFrame bufferSize) :
        _bufferSize(bufferSize)
    {
        _bufferOut.resize(bufferSize * CHANNEL_COUNT);
    }
    virtual ModuleStatus const status() { return ModuleStatus(); }
    virtual void update(ModuleStatus /*status_*/) { }
    virtual void process(Sample const * /*bufferIn*/, const ClockStatus /*time*/) { }
    virtual void gate(ClockStatus /*time*/) { }
    Sample const * output() { return _bufferOut.data(); }

protected:
    Buffer _bufferOut;
    nFrame _bufferSize;
    ClockStatus _time;
    nFrame _left = 0;
    nFrame _right = 0;
    _SampleMeter _outMeterL;
    _SampleMeter _outMeterR;
};

#endif // ABSTRACTMODULE_H
