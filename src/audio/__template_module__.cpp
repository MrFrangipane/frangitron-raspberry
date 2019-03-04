#include "kicksynth.h"

void KickSynth::update(ModuleStatus status)
{
}

const ModuleStatus KickSynth::status()
{
    ModuleStatus status_;

    status_.empty = false; // Will be updated by engine

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.is_clipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void KickSynth::process(Sample const * bufferIn, const nFrame /*time*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) 
    {
        _bufferOut[_left] = bufferIn[_left];
        _bufferOut[_right] = bufferIn[_right];

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}
