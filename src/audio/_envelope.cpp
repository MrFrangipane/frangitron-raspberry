#include "_envelope.h"
#include <iostream>


float _Envelope::value(nFrame time)
{
    if( _lastGate == 0 ) // Prevents startup gate
        return 0.0;

    float elapsed = (float)(time - _lastGate) / SAMPLE_RATE;

    if( elapsed > 0.0 && elapsed <= _attack) // Attack
    {
        return lerp(_valueAtGate, 1.0, elapsed / _attack);
    }
    else if( elapsed > _attack && elapsed <= _attack + _decay) // Decay
    {
        return lerp(1.0, _sustain, (elapsed - _attack) / _decay);
    }
    else if( elapsed > _attack + _decay && elapsed <= _attack + _decay + _hold) // Hold
    {
        return _sustain;
    }
    else if( elapsed > _attack + _decay + _hold && elapsed <= _attack + _decay + _hold + _release) // Release
    {
        return lerp(_sustain, 0.0, (elapsed - _attack - _decay - _hold) / _release);
    }

    return 0.0;
}


void _Envelope::gate(nFrame time)
{
    _lastGate = time;
    _valueAtGate = value(time);
}
