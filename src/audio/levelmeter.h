#ifndef LEVELMETER_H
#define LEVELMETER_H

#include "cmath"
#include "typedefs.h"
#include "audio/abstractmodule.h"

struct _LevelMeterStatus {
    float rmsL = 0.0;
    float rmsR = 0.0;
    float rmsInstantL = 0.0;
    float rmsInstantR = 0.0;
};

class LevelMeter : public AbstractModule
{
public:
    LevelMeter() : AbstractModule(0) { }
    Status const status() override;
    void update(Status /*status_*/) override { }
    void process(Sample const * /*bufferIn*/, const nFrame /*time*/) override { }
    void stepComputations(Sample left, Sample right);
private:
    _LevelMeterStatus _status;
    void _endComputations();
    int _nStep = 0;
    int _nAverage = 0;
    float _instantL = 0.0;
    float _instantR = 0.0;
    float _averageL = 0.0;
    float _averageR = 0.0;
};

#endif // LEVELMETER_H
