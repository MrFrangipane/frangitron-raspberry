#include "_samplemeter.h"


void _SampleMeter::stepCompute(Sample sample)
{
    _sample = sample;
    if( _isAWeighted )
        _sample = _a_weighting.process(_sample);

    _sum += _sample * _sample;
    _nStep++;

    if( _nStep > LEVEL_METER_RMS_SAMPLE_COUNT ) {
        _endCompute();
    }
}

void _SampleMeter::_endCompute()
{
    // Value
    rms.instant = gain2dB(_sum / (float)_nStep);
    _average = (_average * _nAverage + _sum / (float)_nStep) / ((float)_nAverage + 1.0);
    _nAverage++;

    if( _nAverage > LEVEL_METER_AVERAGE_CYCLE_COUNT ) {
        rms.average = gain2dB(_average);
        _nAverage = 0;
    }

    // Clipping
    if( _isClipping && _clippingPersistCounter <= CLIPPING_PERSIST_BUFFER_COUNT) {
        _clippingPersistCounter++;
    } else {
        _clippingPersistCounter = 0;
        _isClipping = false;
    }
    if( rms.instant >= CLIPPING_THRESOLD ) {
        _isClipping = true;
    }

    // Reset
    _sum = 0.0;
    _nStep = 0;
}
