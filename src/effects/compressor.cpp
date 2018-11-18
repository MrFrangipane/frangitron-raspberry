#include "compressor.h"

void Compressor::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    _time = time;
    _rmsMono = fmax(_levelMeter.rmsInstantL, _levelMeter.rmsInstantR);

    if( _rmsMono >= threshold ) {
        level = fmax(1.0 / _ratio, fmin(1.0, level - attack));
    } else {
        level = fmax(1.0 / _ratio, fmin(1.0, level + release));
    }

    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        _levelMeter.stepComputations(bufferIn[_left], bufferIn[_right]);

        bufferOut[_left] = bufferIn[_left] * level;
        bufferOut[_right] = bufferIn[_right] * level;

        _time++;
    }
}
