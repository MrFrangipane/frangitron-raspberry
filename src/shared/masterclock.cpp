#include "masterclock.h"
#include <iostream>


void MasterClock::_update()
{
    _status.seconds = (double)_status.frame / SAMPLE_RATE;

    _status.pulse = _pulsePerSecond * (double)_status.frame / SAMPLE_RATE;
    _status.enginePulse = _pulsePerSecond * (double)_status.engineFrame / SAMPLE_RATE;

    _status.step = 4 * _status.pulse / MIDI_PULSE_PER_BEAT;
    _status.beat = _status.step / 4;
    _status.bar = _status.beat / 4;

    _status.barAsFrame = _status.bar * 4 * MIDI_PULSE_PER_BEAT;
}

void MasterClock::setTempo(float tempo)
{
    _status.tempo = tempo;
    _pulsePerSecond = (_status.tempo / 60.0) * MIDI_PULSE_PER_BEAT;
    _status.framePerBar = SAMPLE_RATE / (_status.tempo / 60.0);
    _update();
}

void MasterClock::incrementFrame(nFrame count)
{
    _status.engineFrame += count;
    if( _status.isPlaying ) _status.frame += count;

    _update();
}
