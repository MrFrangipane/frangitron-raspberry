#include "compressor.h"

const ModuleStatus Compressor::status() {
    ModuleStatus status_;

    status_.params[0].name = "Attack";
    status_.params[0].value = _attack;
    status_.params[0].min = 0.001;
    status_.params[0].max = 1.0;
    status_.params[0].step = 0.001;
    status_.params[0].visible = true;

    status_.params[1].name = "Release";
    status_.params[1].value = _release;
    status_.params[1].min = 0.001;
    status_.params[1].max = 1.0;
    status_.params[1].step = 0.001;
    status_.params[1].visible = true;

    status_.params[2].value = _ratio;
    status_.params[2].name = "Ratio";
    status_.params[2].min = 1.0;
    status_.params[2].max = 10.0;
    status_.params[2].step = 1.0;
    status_.params[2].visible = true;

    status_.params[3].name = "Threshold";
    status_.params[3].value = _threshold;
    status_.params[3].min = -100.0;
    status_.params[3].max = 0.0;
    status_.params[3].step = 0.1;
    status_.params[3].visible = true;

    status_.params[4].name = "Gain";
    status_.params[4].value = gain2dB(_gain);
    status_.params[4].min = -50.0;
    status_.params[4].max = 20.0;
    status_.params[4].step = 0.1;
    status_.params[4].visible = true;

    status_.params[5].name = "Gate";
    status_.params[5].value = _gate;

    status_.params[6].name = "level";
    status_.params[6].value = _level;

    status_.params[7].name = "RmsIn";
    status_.params[7].value = fmax(_inMeterL.rms.average, _inMeterR.rms.average);

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);

    return status_;
}

void Compressor::update(ModuleStatus status) {
    _threshold = status.params[3].value;
    _attack = status.params[0].value;
    _release = status.params[1].value;
    _ratio = (int)status.params[2].value;
    _gain = dB2Gain(status.params[4].value);
}

void Compressor::process(Sample const * bufferIn, const nFrame /*time*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        // TRESHOLD
        if( _nCycles > COMPRESSOR_UPDATE_SAMPLE_COUNT ) {
            _rmsIn = fmax(_inMeterL.rms.instant, _inMeterR.rms.instant);

            _levelPrevious = _levelTarget;
            float increment = 0.0;

            if( _rmsIn >= _threshold ) {
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

        _inMeterL.stepCompute(bufferIn[_left]);
        _inMeterR.stepCompute(bufferIn[_right]);

        _level = lerp(_levelPrevious, _levelTarget, (float)_nCycles / COMPRESSOR_UPDATE_SAMPLE_COUNT);

        _bufferOut[_left] = bufferIn[_left] * _level * _gain;
        _bufferOut[_right] = bufferIn[_right] * _level * _gain;

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);

        _nCycles++;
    }
}
