#include "levelmeter.h"

ModuleStatus const LevelMeter::status()
{
    ModuleStatus status_;

    status_.empty = false; // Will be updated by engine

    status_.params[0].name = "rmsL";
    status_.params[0].value = _meterL.rms.average;
    status_.params[1].name = "rmsR";
    status_.params[1].value = _meterR.rms.average;
    status_.params[2].name  = "rmsInstantL";
    status_.params[2].value = _meterL.rms.instant;
    status_.params[3].name  = "rmsInstantR";
    status_.params[3].value = _meterR.rms.instant;

    status_.params[4].name  = "clip tresh";
    status_.params[4].visible = true;
    status_.params[4].min = 0.0;
    status_.params[4].max = 1.0;
    status_.params[4].value = _clip_tresh;

    status_.params[5].name  = "is_clipping";
    status_.params[5].min = 0.0;
    status_.params[5].max = 1.0;
    status_.params[5].value = (float)_is_clipping;

    return status_;
}


void LevelMeter::update(ModuleStatus status) {
    _clip_tresh = status.params[4].value;
}


void LevelMeter::process(const Sample *bufferIn, const nFrame /*time*/)
{
    if( _is_clipping && _clip_release <= CLIPPING_RELEASE_BUFFER_COUNT) {
        _clip_release++;
    } else {
        _clip_release = 0;
        _is_clipping = false;
    }

    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        _meterL.stepCompute(bufferIn[_left]);
        _meterR.stepCompute(bufferIn[_right]);

        if( bufferIn[_left] >= _clip_tresh || bufferIn[_right] >= _clip_tresh ) {
            _is_clipping = true;
        }

        _bufferOut[_left] = bufferIn[_left];
        _bufferOut[_right] = bufferIn[_right];
    }
}
