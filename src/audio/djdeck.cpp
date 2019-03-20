#include "djdeck.h"


void DjDeck::update(ModuleStatus status)
{
}

const ModuleStatus DjDeck::status()
{
    ModuleStatus status_;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.isClipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void DjDeck::process(Sample const * bufferIn, const nFrame /*time*/)
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
