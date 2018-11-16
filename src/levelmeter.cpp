#include "levelmeter.h"

void LevelMeter::bufferBegin()
{
    _instantL = 0.0;
    _instantR = 0.0;
    _nStep = 0;
}

void LevelMeter::bufferStep(Sample left, Sample right)
{
    _instantL += left * left;
    _instantR += right * right;
    _nStep++;
}

void LevelMeter::bufferEnd()
{
    rmsInstantL = 10 * log(2.0 * _instantL / (float)_nStep);
    rmsInstantR = 10 * log(2.0 * _instantR / (float)_nStep);

    _averageL = (_averageL * _nAverage + _instantL / (float)_nStep) / ((float)_nAverage + 1.0);
    _averageR = (_averageR * _nAverage + _instantR / (float)_nStep) / ((float)_nAverage + 1.0);

    if( _nAverage >= 30 ) {
        _nAverage = 0;
        rmsL = 10 * log(2.0 * _averageL);
        rmsR = 10 * log(2.0 * _averageR);
    }

    _nAverage++;
}
