#include "filter.h"

void Filter::update(ModuleStatus status_)
{
    // PARAMS[1] : CUTOFF
    if( status_.params[1].value < 0 ) {
        _mode = FilterMode::LOWPASS;
        _lowPassL.setCutoff(1.0 + status_.params[1].value);
        _lowPassR.setCutoff(1.0 + status_.params[1].value);
        _hiPassL.setCutoff(0.0);
        _hiPassR.setCutoff(0.0);
    }
    else if ( status_.params[1].value > 0 ) {
        _mode = FilterMode::HIPASS;
        _lowPassL.setCutoff(1.0);
        _lowPassR.setCutoff(1.0);
        _hiPassL.setCutoff(status_.params[1].value);
        _hiPassR.setCutoff(status_.params[1].value);
    }
    else {
        _mode = FilterMode::PASSTHROUGH;
        _lowPassL.setCutoff(1.0);
        _lowPassR.setCutoff(1.0);
        _hiPassL.setCutoff(0.0);
        _hiPassR.setCutoff(0.0);
    }

    // PARAMS[3] : RESONANCE
    _lowPassL.setResonance(status_.params[3].value);
    _lowPassR.setResonance(status_.params[3].value);
    _hiPassL.setResonance(status_.params[3].value);
    _hiPassR.setResonance(status_.params[3].value);
}

const ModuleStatus Filter::status()
{
    ModuleStatus status_;

    status_.empty = false;

    status_.params[1].name = "Cutoff";
    status_.params[1].min = -1.0;
    status_.params[1].max = 1.0;
    status_.params[1].step = 0.01;
    status_.params[1].visible = true;

    if( _mode == FilterMode::LOWPASS ) {
        status_.params[1].value = _lowPassL.cutoff() - 1.0;
    }
    else if( _mode == FilterMode::HIPASS ) {
        status_.params[1].value = _hiPassL.cutoff();
    }
    else {
        status_.params[1].value = 0.0;
    }

    status_.params[3].name = "Resonance";
    status_.params[3].min = 0.0;
    status_.params[3].max = 0.6;
    status_.params[3].step = 0.003;
    status_.params[3].visible = true;
    status_.params[3].value = _hiPassL.resonance();

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.is_clipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void Filter::process(Sample const * bufferIn, const nFrame /*time*/)
{
    switch (_mode) {

    case FilterMode::LOWPASS:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = _lowPassL.process(bufferIn[_left]);
            _bufferOut[_right] = _lowPassR.process(bufferIn[_right]);

            _outMeterL.stepCompute(_bufferOut[_left]);
            _outMeterR.stepCompute(_bufferOut[_right]);
        }
        break;

    case FilterMode::HIPASS:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = _hiPassL.process(bufferIn[_left]);
            _bufferOut[_right] = _hiPassR.process(bufferIn[_right]);

            _outMeterL.stepCompute(_bufferOut[_left]);
            _outMeterR.stepCompute(_bufferOut[_right]);
        }
        break;

    case FilterMode::PASSTHROUGH:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = bufferIn[_left] * ATTENUATION;
            _bufferOut[_right] = bufferIn[_right] * ATTENUATION;

            _outMeterL.stepCompute(_bufferOut[_left]);
            _outMeterR.stepCompute(_bufferOut[_right]);
        }
        break;

    default:
        break;
    }
}
