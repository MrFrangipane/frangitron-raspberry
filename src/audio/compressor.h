#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/levelmeter.h"

class Compressor : public AbstractModule
{
public:
    Compressor(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize),
        _levelMeter(bufferSize)
    { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    nFrame _nCycles = 0;
    nFrame _time = 0;
    nFrame _nLerp = 0;
    float _threshold = -64.0;
    float _attack = .08;
    float _release = .02;
    float _ratio = 4.0;
    float _gain = 4.0;
    float _level = 1.0;
    float _levelPrevious = 1.0;
    float _levelTarget = 1.0;
    float _rmsMono = 0.0;
    bool _gate = false;
    LevelMeter _levelMeter;
};

#endif // COMPRESSOR_H
