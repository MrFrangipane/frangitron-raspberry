#include "masterclock.h"
#include <iostream>


void MasterClock::_update()
{
    double ppqnCount_ = float(_frame) * _ppqnPerFrame; // HACKY WHACKY 120 BPM

    int note = ppqnCount_ / 24;
    int bar = note / 4;

    _status.bar = bar + (double)(note % 4) * 0.1;
    _status.seconds = (double)_frame / SAMPLE_RATE;
    _status.sequenceStep = ppqnCount_ / 6.0;
    _status.frame = _frame;
    _status.engineFrame = _engineFrame;

    _status.isPlaying = _isPlaying;
    _status.isAtBar = fmod(_status.sequenceStep, 4.0) == 0.0;

    if( _status.isAtBar )
        _status.lastBarFrame = _frame;

    if( isPlaying() && false ) // HACKY WHACKY 120 BPM
    {
        _tempoAveraging += _ppqnPerFrame / 24 * SAMPLE_RATE * 60;
        _tempoCycles += 1;
        if( _status.seconds - _lastTempoAveraging > MIDI_TEMPO_AVERAGE_DURATION )
        {
            _lastTempoAveraging = _status.seconds;
            _status.tempo = _tempoAveraging / _tempoCycles;
            _tempoAveraging = 0;
            _tempoCycles = 0;
            _status.framePerBar = SAMPLE_RATE / (_status.tempo / 60.0) * 4.0;
        }
    }
    else
    {
        _lastTempoAveraging = 0;
        _status.tempo = 120.0;
        _status.framePerBar = SAMPLE_RATE / (_status.tempo / 60.0) * 4.0;
    }
}

void MasterClock::incrementFrame(nFrame count)
{
    _engineFrame += count;
    if( _isPlaying ) _frame += count;

    _update();
}

void MasterClock::incrementPpqn()
{
    // HACKY WHACKY 120 BPM ---

    //nFrame elapsed = _engineFrame - _lastPpqnUpdate;
    //_lastPpqnUpdate = _engineFrame;
    _ppqnPerFrame = 0.001;

    //_ppqnCount += 1;
    _update();

    // ------------------------
}
