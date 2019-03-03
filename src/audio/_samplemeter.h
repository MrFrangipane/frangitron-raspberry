#ifndef _SAMPLEMETER_H
#define _SAMPLEMETER_H

#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/audiohelpers.h"
#include "_samplefilter.h"

struct _SampleMeterStatus {
    float average = 0.0;
    float instant = 0.0;
};

class _SampleMeter
{
public:
    _SampleMeter();
    void stepCompute(Sample sample);
    void setWeighted(bool isWeighted) { _isWeighted = isWeighted; }
    bool isClipping() { return _is_clipping; }
    _SampleMeterStatus rms;

private:
    void _endCompute();
    Sample _sample;
    int _nStep = 0;
    int _nAverage = 0;
    float _sum = 0.0;
    float _average = 0.0;
    bool _isWeighted = true;
    bool _is_clipping = false;
    int _clip_persist_counter = 0;
    _SampleFilter _filter_hi;
    _SampleFilter _filter_low;
};

#endif // _SAMPLEMETER_H
