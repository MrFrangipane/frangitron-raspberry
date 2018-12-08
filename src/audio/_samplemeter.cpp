#include "_samplemeter.h"

void _SampleMeter::stepCompute(Sample sample)
{
    _instant += sample * sample;
    _nStep++;

    if( _nStep > LEVEL_METER_RMS_SAMPLE_COUNT ) {
        _endCompute();
    }
}

void _SampleMeter::_endCompute()
{
    rms.instant = gain2dB(_instant / (float)_nStep);
    _average = (_average * _nAverage + _instant / (float)_nStep) / ((float)_nAverage + 1.0);
    _nAverage++;

    if( _nAverage > LEVEL_METER_AVERAGE_CYCLE_COUNT ) {
        rms.average = gain2dB(_average);
        _nAverage = 0;
    }

    _instant = 0.0;
    _nStep = 0;
}
