#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "shared/typedefs.h"
#include "shared/structures.h"
#include "audio/_samplemeter.h"

class AbstractModule
{
public:
    AbstractModule(const nFrame bufferSize) :
        _bufferSize(bufferSize)
    {
        _bufferOut.reserve(bufferSize * 2);
    }
    virtual ModuleStatus const status() { return ModuleStatus(); }
    virtual void update(ModuleStatus /*status_*/) { }
    virtual void process(Sample const * /*bufferIn*/, const nFrame /*time*/) { }
    virtual void gate(nFrame time) { _last_gate = time; }
    Sample const * output() { return _bufferOut.data(); }

protected:
    Buffer _bufferOut;
    nFrame _bufferSize;
    nFrame _time = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _last_gate = 500000;  // hacky TODO: find out why gate is triggered at startup
    _SampleMeter _outMeterL;
    _SampleMeter _outMeterR;
};

#endif // ABSTRACTMODULE_H
