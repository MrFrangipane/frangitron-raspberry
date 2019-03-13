#ifndef _SAMPLEMETER_H
#define _SAMPLEMETER_H

#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/audiohelpers.h"
#include "_aweighting.h"


struct _SampleMeterStatus {
    float average = -60.0;
    float instant = -60.0;
};

class _SampleMeter
{
public:
    _SampleMeter() { }
    void stepCompute(Sample sample);
    void setAWeighted(bool isAWeighted) { _isAWeighted = isAWeighted; }
    bool isClipping() { return _isClipping; }
    _SampleMeterStatus rms;

private:
    void _endCompute();
    Sample _sample;
    int _nStep = 0;
    int _nAverage = 0;
    float _sum = 0.0;
    float _average = 0.0;
    bool _isAWeighted = true;
    bool _isClipping = false;
    int _clippingPersistCounter = 0;
    _AWeighting _a_weighting;
};

#endif // _SAMPLEMETER_H
