#include "compressor.h"

Status const Compressor::status() {
    Status status_;

    status_["level"] = _level;
    status_["gate"] = _gate;
    status_["threshold"] = _threshold;
    status_["attack"] = _attack;
    status_["release"] = _release;
    status_["ratio"] = _ratio;
    status_["gain"] = _gain;
    status_["rms"] = std::fmax(_levelMeter.status().at("rmsL"), _levelMeter.status().at("rmsR"));

    return status_;
}

void Compressor::update(Status status) {
    _threshold = status["threshold"];
    _attack = status["attack"];
    _release = status["release"];
    _ratio = status["ratio"];
    _gain = status["gain"];
}

void Compressor::process(Sample const * bufferIn, const nFrame /*time*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        // TRESHOLD
        if( _nCycles > COMPRESSOR_UPDATE_SAMPLE_COUNT ) {
            _rmsMono = fmax(_levelMeter.status().at("rmsInstantL"), _levelMeter.status().at("rmsInstantR"));

            _levelPrevious = _levelTarget;
            float increment = 0.0;

            if( _rmsMono >= _threshold ) {
                _gate = true;
                increment = - COMPRESSOR_UPDATE_SAMPLE_COUNT / (_attack * SAMPLE_RATE);
            } else {
                _gate = false;
                increment = COMPRESSOR_UPDATE_SAMPLE_COUNT / (_release * SAMPLE_RATE);
            }

            _levelTarget = fmax(1.0 / _ratio, fmin(1.0, _levelPrevious + increment));
            _nCycles = 0;
        }

        // APPLY
        _left = i * 2;
        _right = _left + 1;
        _levelMeter.stepComputations(bufferIn[_left], bufferIn[_right]);
        _level = lerp(_levelPrevious, _levelTarget, (float)_nCycles / COMPRESSOR_UPDATE_SAMPLE_COUNT);

        _bufferOut[_left] = bufferIn[_left] * _level * _gain;
        _bufferOut[_right] = bufferIn[_right] * _level * _gain;

        _nCycles++;
    }
}
