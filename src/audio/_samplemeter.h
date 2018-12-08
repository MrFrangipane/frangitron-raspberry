#ifndef _SAMPLEMETER_H
#define _SAMPLEMETER_H

#include "typedefs.h"

struct _SampleMeterStatus {
    float average = 0.0;
    float instant = 0.0;
};

class _SampleMeter
{
public:
    _SampleMeter() { }
    void stepCompute(Sample sample);
    _SampleMeterStatus rms;

private:
    void _endCompute();
    int _nStep = 0;
    int _nAverage = 0;
    float _instant = 0.0;
    float _average = 0.0;
};

#endif // _SAMPLEMETER_H
