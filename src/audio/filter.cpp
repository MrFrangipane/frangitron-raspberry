#include "filter.h"

void Filter::update(ModuleStatus status_)
{
    // PARAMS[0] : CUTOFF
    if( status_.params[0].value < 0 ) {
        _mode = FilterMode::LOWPASS;
        _lowPassL.setCutoff(1.0 + status_.params[0].value);
        _lowPassR.setCutoff(1.0 + status_.params[0].value);
        _hiPassL.setCutoff(0.0);
        _hiPassR.setCutoff(0.0);
    }
    else if ( status_.params[0].value > 0 ) {
        _mode = FilterMode::HIPASS;
        _lowPassL.setCutoff(1.0);
        _lowPassR.setCutoff(1.0);
        _hiPassL.setCutoff(status_.params[0].value);
        _hiPassR.setCutoff(status_.params[0].value);
    }
    else {
        _mode = FilterMode::PASSTHROUGH;
        _lowPassL.setCutoff(1.0);
        _lowPassR.setCutoff(1.0);
        _hiPassL.setCutoff(0.0);
        _hiPassR.setCutoff(0.0);
    }

    // PARAMS[1] : RESONANCE
    _lowPassL.setResonance(status_.params[1].value);
    _lowPassR.setResonance(status_.params[1].value);
    _hiPassL.setResonance(status_.params[1].value);
    _hiPassR.setResonance(status_.params[1].value);
}

const ModuleStatus Filter::status()
{
    ModuleStatus status_;

    status_.params[0].name = "Cutoff";
    status_.params[0].min = -1.0;
    status_.params[0].max = 1.0;
    status_.params[0].step = 0.001;
    status_.params[0].visible = true;

    if( _mode == FilterMode::LOWPASS ) {
        status_.params[0].value = _lowPassL.cutoff() - 1.0;
    }
    else if( _mode == FilterMode::HIPASS ) {
        status_.params[0].value = _hiPassL.cutoff();
    }
    else {
        status_.params[0].value = 0.0;
    }

    status_.params[1].name = "Resonance";
    status_.params[1].min = 0.0;
    status_.params[1].max = 0.6;
    status_.params[1].step = 0.001;
    status_.params[1].visible = true;
    status_.params[1].value = _hiPassL.resonance();

    status_.levelOut.name = "RmsOut";
    status_.levelOut.value = fmax(_outMeterL.rms.average, _outMeterR.rms.average);

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

            _bufferOut[_left] = bufferIn[_left] * FEEDBACK_FACTOR;
            _bufferOut[_right] = bufferIn[_right] * FEEDBACK_FACTOR;

            _outMeterL.stepCompute(_bufferOut[_left]);
            _outMeterR.stepCompute(_bufferOut[_right]);
        }
        break;

    default:
        break;
    }
}
