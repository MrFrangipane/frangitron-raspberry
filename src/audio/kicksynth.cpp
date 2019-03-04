#include "kicksynth.h"
#include "iostream"

KickSynth::KickSynth(const nFrame bufferSize) : AbstractModule(bufferSize)
{
    _pitch = 120.0;

    _sidechain_amount = 0.6;

    _hipass.setMode(HIPASS);
    _hipass.setCutoff(0.2);
    _hipass.setResonance(0.0);

    _envelope_amplitude.setAttack(0.0005);
    _envelope_amplitude.setDecay(0.15);
    _envelope_amplitude.setSustain(0.0);
    _envelope_amplitude.setRelease(0.005);

    _envelope_pitch.setAttack(0.0);
    _envelope_pitch.setDecay(_envelope_amplitude.decay() * 0.66);
    _envelope_pitch.setSustain(0.4);
    _envelope_pitch.setRelease(10.0);
}


void KickSynth::update(ModuleStatus status)
{
    _pitch = status.params[0].value;
}

const ModuleStatus KickSynth::status()
{
    ModuleStatus status_;

    status_.empty = false; // Will be updated by engine

    status_.params[0].name = "Pitch";
    status_.params[0].value = _pitch;
    status_.params[0].min = 50;
    status_.params[0].max = 200;
    status_.params[0].step = 1;
    status_.params[0].visible = true;

    status_.params[5].name = "Amplitude";
    status_.params[5].value = _amplitude;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.is_clipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void KickSynth::process(Sample const * bufferIn, const nFrame time)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;
        float elapsed = (float)(time + i - _last_gate) / SAMPLE_RATE;

        if( elapsed <= _envelope_amplitude.duration() )
        {
            _amplitude = _envelope_amplitude.value(time + i);
            Sample _sample = std::cos(elapsed * 2.0 * M_PI * _pitch * _envelope_pitch.value(time + i)) * _amplitude;
            _sample = _hipass.process(_sample) * 0.7;

            _bufferOut[_left] = _sample + bufferIn[_left] * (1.0 - _amplitude * _sidechain_amount) * 0.8;
            _bufferOut[_right] = _sample + bufferIn[_left] * (1.0 - _amplitude * _sidechain_amount) * 0.8;

        } else
        {
            _amplitude = 0.0;
            _bufferOut[_left] = bufferIn[_left] * 0.8;
            _bufferOut[_right] = bufferIn[_right] * 0.8;
        }

        _outMeterL.stepCompute(_bufferOut[_left]);
        _outMeterR.stepCompute(_bufferOut[_right]);
    }
}


void KickSynth::gate(nFrame time)
{
    _last_gate = time;
    _envelope_amplitude.gate(time);
    _envelope_pitch.gate(time);
}
