#include "compressor.h"
#include <iostream>

CompressorStatus Compressor::status() {
    CompressorStatus status_;

    status_.level = _level;
    status_.gate = _gate;
    status_.threshold = threshold;
    status_.attack = attack;
    status_.release = release;
    status_.ratio = ratio;
    status_.gain = gain;

    return status_;
}

void Compressor::setStatus(CompressorStatus status_) {
    threshold = status_.threshold;
    attack = status_.attack;
    release = status_.release;
    ratio = status_.ratio;
    gain = status_.gain;
}

void Compressor::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        // TRESHOLD
        if( _nCycles > COMPRESSOR_UPDATE_SAMPLE_COUNT ) {
            _rmsMono = fmax(_levelMeter.rmsInstantL, _levelMeter.rmsInstantR);

            _levelPrevious = _levelTarget;
            float increment = 0.0;

            if( _rmsMono >= threshold ) {
                _gate = true;
                increment = - COMPRESSOR_UPDATE_SAMPLE_COUNT / (attack * SAMPLE_RATE);
            } else {
                _gate = false;
                increment = COMPRESSOR_UPDATE_SAMPLE_COUNT / (release * SAMPLE_RATE);
            }

            _levelTarget = fmax(1.0 / ratio, fmin(1.0, _levelPrevious + increment));
            _nCycles = 0;
        }

        // APPLY
        _left = i * 2;
        _right = _left + 1;
        _levelMeter.stepComputations(bufferIn[_left], bufferIn[_right]);
        _level = lerp(_levelPrevious, _levelTarget, (float)_nCycles / COMPRESSOR_UPDATE_SAMPLE_COUNT);

        bufferOut[_left] = bufferIn[_left] * _level * gain;
        bufferOut[_right] = bufferIn[_right] * _level * gain;

        _nCycles++;
    }
}
