#include "sampleplayer.h"
#include <iostream>


void SamplePlayer::update(ModuleStatus status)
{
    _sampleIndex = status.params[2].value;
}

const ModuleStatus SamplePlayer::status()
{
    ModuleStatus status_;

    status_.params[2].name = "Sample";
    status_.params[2].value = _sampleIndex;
    status_.params[2].min = 0;
    status_.params[2].max = 3;
    status_.params[2].step = 1;
    status_.params[2].isVisible = true;

    status_.params[5].name = "Amplitude";
    status_.params[5].value = _amplitude;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.isClipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void SamplePlayer::process(Sample const * bufferIn, const ClockStatus time)
{
    for( nFrame i = 0; i < _bufferSize; i++ )
    {
        _left = i * 2;
        _right = _left + 1;
        _amplitude = _envAmplitude.value(time.engineFrame + i);
        nFrame elapsedFrames = time.engineFrame + i - _envAmplitude.lastGate();
        float elapsedSeconds = (float)elapsedFrames / SAMPLE_RATE;

        if( elapsedSeconds <= _envAmplitude.duration() )
        {
            _amplitude = _envAmplitude.value(time.engineFrame + i);
            _bufferOut[_left] = bufferIn[_left] + _sampleBank->left(_sampleIndex, elapsedFrames) * _amplitude;
            _bufferOut[_right] = bufferIn[_right] + _sampleBank->right(_sampleIndex, elapsedFrames) * _amplitude;
        }
        else
        {
            _amplitude = 0.0;
            _bufferOut[_left] = bufferIn[_left];
            _bufferOut[_right] = bufferIn[_right];
        }

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}

void SamplePlayer::gate(ClockStatus time)
{
    _envAmplitude.gate(time.engineFrame);
    _envSidechain.gate(time.engineFrame);
}
