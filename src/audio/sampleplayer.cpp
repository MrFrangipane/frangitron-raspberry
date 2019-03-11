#include "sampleplayer.h"


void SamplePlayer::update(ModuleStatus status)
{
}

const ModuleStatus SamplePlayer::status()
{
    ModuleStatus status_;

    status_.empty = false; // Will be updated by engine

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.is_clipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void SamplePlayer::process(Sample const * bufferIn, const nFrame /*time*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;
        //float elapsed = (float)(time + i - _last_gate) / SAMPLE_RATE;
        _bufferOut[_left] = bufferIn[_left];
        _bufferOut[_right] = bufferIn[_right];

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}

void SamplePlayer::gate(nFrame time)
{
    _envelope_amplitude.gate(time);
    _envelope_sidechain.gate(time);
}
