#include "_adr.h"

float _ADR::value(nFrame time)
{
    float elapsed = (float)(time - _last_gate) / SAMPLE_RATE;


    if( elapsed > 0.0 && elapsed <= _attack) // Attack
    {
        return lerp(0.0, 1.0, elapsed / _attack);
    }
    else if( elapsed > _attack && elapsed <= _attack + _decay) // Decay
    {
        return lerp(1.0, _sustain, (elapsed - _attack) / _decay);
    }
    else if( elapsed > _attack + _decay && elapsed <= _attack + _decay + _release) // Release
    {
        return lerp(_sustain, 0.0, (elapsed - _attack - _decay) / _release);
    }

    return 0.0;
}
