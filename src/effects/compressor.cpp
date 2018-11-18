#include "compressor.h"

void Compressor::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        // TRESHOLD
        if( _nCycles > COMPRESSOR_UPDATE_SAMPLE_COUNT ) {
            _rmsMono = fmax(_levelMeter.rmsInstantL, _levelMeter.rmsInstantR);

            _levelPrevious = _levelTarget;

            if( _rmsMono >= threshold ) {
                _levelTarget = fmax(1.0 / _ratio, fmin(1.0, _levelPrevious - attack));
            } else {
                _levelTarget = fmax(1.0 / _ratio, fmin(1.0, _levelPrevious + release));
            }

            _nCycles = 0;
        }

        // APPLY
        _left = i * 2;
        _right = _left + 1;
        _level = lerp(_levelPrevious, _levelTarget, (float)_nLerp / COMPRESSOR_LERP_SAMPLE_COUNT);
        _levelMeter.stepComputations(bufferIn[_left], bufferIn[_right]);

        bufferOut[_left] = bufferIn[_left] * _level;
        bufferOut[_right] = bufferIn[_right] * _level;

        _nCycles++;
        _nLerp++;

        if( _nLerp > COMPRESSOR_LERP_SAMPLE_COUNT ) { _nLerp = 0; }
    }
}
