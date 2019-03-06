#include "kicksynth.h"
#include "iostream"

KickSynth::KickSynth(const nFrame bufferSize) : AbstractModule(bufferSize)
{
    _pitch = 600.0;

    _sidechain_amount = 0.25;

    _hipass_1.setMode(HIPASS);
    _hipass_1.setCutoff(0.18);
    _hipass_1.setResonance(0.0);

    _hipass_2.setMode(HIPASS);
    _hipass_2.setCutoff(0.18);
    _hipass_2.setResonance(0.0);

    _envelope_amplitude.setAttack(0.25 / ATTACK_FACTOR);
    _envelope_amplitude.setDecay(0.0);
    _envelope_amplitude.setSustainLevel(1.0);
    _envelope_amplitude.setHold(0.07);
    _envelope_amplitude.setRelease(0.20);

    _envelope_pitch.setAttack(0.0);
    _envelope_pitch.setDecay(0.0);
    _envelope_pitch.setSustainLevel(1.0);
    _envelope_pitch.setHold(0.05);
    _envelope_pitch.setRelease(0.5);
}

const ModuleStatus KickSynth::status()
{
    ModuleStatus status_;

    status_.empty = false; // Will be updated by engine
/*
    status_.params[0].name = "A";
    status_.params[0].value = _envelope_amplitude.attack() * ATTACK_FACTOR;
    status_.params[0].min = 0;
    status_.params[0].max = 1;
    status_.params[0].step = 0.01;
    status_.params[0].visible = true;

    status_.params[1].name = "D";
    status_.params[1].value = _envelope_amplitude.decay();
    status_.params[1].min = 0;
    status_.params[1].max = 0.2;
    status_.params[1].step = 0.01;
    status_.params[1].visible = true;

    status_.params[2].name = "S";
    status_.params[2].value = _envelope_amplitude.sustainLevel();
    status_.params[2].min = 0;
    status_.params[2].max = 1;
    status_.params[2].step = 0.01;
    status_.params[2].visible = true;

    status_.params[3].name = "H";
    status_.params[3].value = _envelope_amplitude.hold();
    status_.params[3].min = 0;
    status_.params[3].max = 0.2;
    status_.params[3].step = 0.01;
    status_.params[3].visible = true;

    status_.params[4].name = "R";
    status_.params[4].value = _envelope_amplitude.release();
    status_.params[4].min = 0;
    status_.params[4].max = 0.1;
    status_.params[4].step = 0.1;
    status_.params[4].visible = true;
    /**/
    status_.params[0].name = "Pitch";
    status_.params[0].value = _pitch;
    status_.params[0].min = 20;
    status_.params[0].max = 200;
    status_.params[0].step = 1;
    status_.params[0].visible = true;

    status_.params[1].name = "Hold";
    status_.params[1].value = _envelope_amplitude.hold();
    status_.params[1].min = 0;
    status_.params[1].max = 0.2;
    status_.params[1].step = 0.001;
    status_.params[1].visible = true;

    status_.params[2].name = "Release";
    status_.params[2].value = _envelope_amplitude.release();
    status_.params[2].min = 0.0;
    status_.params[2].max = 0.5;
    status_.params[2].step = 0.001;
    status_.params[2].visible = true;

    status_.params[3].name = "Hi Pass";
    status_.params[3].value = _hipass_1.cutoff() * 10.0;
    status_.params[3].min = 0.0;
    status_.params[3].max = 3.0;
    status_.params[3].step = 0.001;
    status_.params[3].visible = true;

    status_.params[4].name = "Sidechain";
    status_.params[4].value = _sidechain_amount;
    status_.params[4].min = 0.0;
    status_.params[4].max = 1.0;
    status_.params[4].step = 0.001;
    status_.params[4].visible = true;
//*/

    status_.params[5].name = "Amplitude";
    status_.params[5].value = _amplitude;

    status_.params[6].name = "Pitchitude";
    status_.params[6].value = _pichitude;

    status_.levelOut = fmax(_outMeterL.rms.average, _outMeterR.rms.average);
    status_.is_clipping = _outMeterL.isClipping() || _outMeterR.isClipping();

    return status_;
}

void KickSynth::update(ModuleStatus status)
{
    /*
    _envelope_amplitude.setAttack(status.params[0].value / ATTACK_FACTOR);
    _envelope_amplitude.setDecay(status.params[1].value);
    _envelope_amplitude.setSustainLevel(status.params[2].value);
    _envelope_amplitude.setHold(status.params[3].value);
    _envelope_amplitude.setRelease(status.params[4].value);

    /**/
    _pitch = status.params[0].value;
    _envelope_amplitude.setHold(status.params[1].value);
    _envelope_amplitude.setRelease(status.params[2].value);
    _hipass_1.setCutoff(status.params[3].value / 10.0);
    _hipass_2.setCutoff(status.params[3].value / 10.0);
    _sidechain_amount = status.params[4].value;
    //*/
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
            _pichitude = _envelope_pitch.value(time + i);
            Sample _sample = std::cos(elapsed * 2.0 * M_PI * _pitch * _pichitude) * _amplitude;
            _sample = _hipass_1.process(_sample) * 0.7;
            _sample = _hipass_2.process(_sample) * 0.7;

            _bufferOut[_left] = _sample + bufferIn[_left] * (1.0 - _amplitude * _sidechain_amount) * 0.8;
            _bufferOut[_right] = _sample + bufferIn[_left] * (1.0 - _amplitude * _sidechain_amount) * 0.8;

        } else
        {
            _amplitude = 0.0;
            _pichitude = 0.0;
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
