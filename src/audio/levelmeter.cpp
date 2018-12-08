#include "levelmeter.h"

ModuleStatus const LevelMeter::status()
{
    ModuleStatus status_;

    status_.params[0].name = "rmsL";
    status_.params[0].value = _meterL.rms.average;
    status_.params[1].name = "rmsR";
    status_.params[1].value = _meterR.rms.average;
    status_.params[2].name  = "rmsInstantL";
    status_.params[2].value = _meterL.rms.instant;
    status_.params[3].name  = "rmsInstantR";
    status_.params[3].value = _meterR.rms.instant;

    return status_;
}

void LevelMeter::process(const Sample *bufferIn, const nFrame /*time*/)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        _meterL.stepCompute(bufferIn[_left]);
        _meterR.stepCompute(bufferIn[_right]);

        _bufferOut[_left] = bufferIn[_left];
        _bufferOut[_right] = bufferIn[_right];
    }
}
