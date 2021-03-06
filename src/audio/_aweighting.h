#ifndef _AWEIGHTING_H
#define _AWEIGHTING_H

#define A_WEIGHT_GAIN 7.0795; // +17 dB

#include "shared/constants.h"
#include "audio/_samplefilter.h"


class _AWeighting
{
public:
    _AWeighting();
    Sample process(Sample sample);

private:
    Sample _sample = 0.0;
    _SampleFilter _filterHi;
    _SampleFilter _filterLow1;
    _SampleFilter _filterLow2;
};

#endif // _AWEIGHTING_H
