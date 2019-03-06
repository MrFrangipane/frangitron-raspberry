#ifndef _ENVELOPE_H
#define _ENVELOPE_H

#include <cmath>
#include "shared/constants.h"
#include "shared/typedefs.h"
#include "shared/audiohelpers.h"


class _Envelope
{
public:
    _Envelope() { }
    float value(nFrame time);
    float duration() { return _attack + _decay + _hold + _release; }
    float attack() { return _attack; }
    float decay() { return _decay; }
    float hold() { return _hold; }
    float sustainLevel() { return _sustain; }
    float release() { return _release; }
    void gate(nFrame time);
    void setAttack(float attack) { _attack = std::fmax(0.0, attack); }
    void setDecay(float decay) { _decay = std::fmax(0.0, decay); }
    void setHold(float hold) { _hold = std::fmax(0.0, hold); }
    void setSustainLevel(float sustain) { _sustain = std::fmin(1.0, std::fmax(0.0, sustain)); }
    void setRelease(float release) { _release = std::fmax(0.0, release); }
private:
    nFrame _last_gate = 6000;
    float _value_at_gate = 0.0;
    float _attack = 0.01;
    float _decay = 0.01;
    float _hold = 0.01;
    float _sustain = 0.8;
    float _release = 0.3;
};

#endif // _ENVELOPE_H
