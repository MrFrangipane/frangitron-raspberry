#include "_samplemeter.h"

_SampleMeter::_SampleMeter()
{
    _filter_low.setMode(_SampleFilterMode::LOWPASS);
    _filter_low.setCutoff(0.45);
    _filter_low.setResonance(0);

    _filter_hi.setMode(_SampleFilterMode::HIPASS);
    _filter_hi.setCutoff(0.95);
    _filter_hi.setResonance(0);
}


void _SampleMeter::stepCompute(Sample sample)
{
    _sample = sample;
    if( _isWeighted ) {
        _sample = _filter_low.process(_sample) * 5.556; // remove
        _sample = _filter_hi.process(_sample) * 5.556;  // filters attenuation
        _sample *= MAGIC_LEVEL_COEFF; // Sad magic coeff
    }

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
    if( _is_clipping && _clip_persist_counter <= CLIPPING_PERSIST_BUFFER_COUNT) {
        _clip_persist_counter++;
    } else {
        _clip_persist_counter = 0;
        _is_clipping = false;
    }
    if( rms.instant >= CLIPPING_THRESOLD ) {
        _is_clipping = true;
    }

    // Reset
    _sum = 0.0;
    _nStep = 0;
}
