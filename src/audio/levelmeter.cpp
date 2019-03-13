#include "levelmeter.h"

ModuleStatus const LevelMeter::status()
{
    ModuleStatus status_;

    status_.params[2].name = "Level";
    status_.params[2].value = _level;
    status_.params[2].isVisible = !_isLevelLocked;
    status_.params[2].min = -60.0;
    status_.params[2].max = 6.0;
    status_.params[2].step = 1.0;

    status_.params[5].name = "rmsL";
    status_.params[5].value = _meterL.rms.average;

    status_.params[6].name = "rmsR";
    status_.params[6].value = _meterR.rms.average;

    status_.params[7].name  = "rmsInstantL";
    status_.params[7].value = _meterL.rms.instant;

    status_.params[8].name  = "rmsInstantR";
    status_.params[8].value = _meterR.rms.instant;

    status_.params[9].name  = "is_clipping";
    status_.params[9].value = (float)(_meterL.isClipping() || _meterR.isClipping());

    status_.params[10].name  = "is_locked";
    status_.params[10].value = (float)(_isLevelLocked);

    return status_;
}


void LevelMeter::update(ModuleStatus status)
{
    _isLevelLocked = (bool)status.params[10].value;

    if( !_isLevelLocked ) {
        _level = status.params[2].value;
    }
}


void LevelMeter::process(const Sample *bufferIn, const nFrame /*time*/, const SampleBank * /*sampleBank*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        _bufferOut[_left] = bufferIn[_left] * dB2Gain(_level);
        _bufferOut[_right] = bufferIn[_right] * dB2Gain(_level);

        _meterL.stepCompute(_bufferOut[_left]);
        _meterR.stepCompute(_bufferOut[_right]);
    }
}
