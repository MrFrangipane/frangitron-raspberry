#include "levelmeter.h"

void LevelMeter::stepComputations(Sample left, Sample right)
{
    _instantL += left * left;
    _instantR += right * right;
    _nStep++;

    if( _nStep > LEVEL_METER_RMS_SAMPLE_COUNT ) {
        _endComputations();
    }
}

Status const LevelMeter::status()
{
    Status status_;

    status_["rmsL"] = _status.rmsL;
    status_["rmsR"] = _status.rmsR;
    status_["rmsInstantL"] = _status.rmsInstantL;
    status_["rmsInstantL"] = _status.rmsInstantR;

    return status_;
}

void LevelMeter::_endComputations()
{
    _status.rmsInstantL = 10 * log(2.0 * _instantL / (float)_nStep);
    _status.rmsInstantR = 10 * log(2.0 * _instantR / (float)_nStep);

    _averageL = (_averageL * _nAverage + _instantL / (float)_nStep) / ((float)_nAverage + 1.0);
    _averageR = (_averageR * _nAverage + _instantR / (float)_nStep) / ((float)_nAverage + 1.0);

    _nAverage++;

    if( _nAverage > LEVEL_METER_AVERAGE_CYCLE_COUNT ) {
        _status.rmsL = 10 * log(2.0 * _averageL);
        _status.rmsR = 10 * log(2.0 * _averageR);

        _nAverage = 0;
    }

    _instantL = 0.0;
    _instantR = 0.0;
    _nStep = 0;
}
