#include "masterclock.h"


void MasterClock::_update()
{
    int note = _ppqnCount / 24;
    int bar = note / 4;

    _status.isPlaying = _isPlaying;
    _status.bar = bar + (double)(note % 4) * 0.1;
    _status.seconds = (double)_frame / SAMPLE_RATE;
    _status.sequenceStep = _ppqnCount / 6;
    _status.frame = _frame;
    _status.engineFrame = _engineFrame;

}

void MasterClock::incrementFrame(nFrame count)
{
    _engineFrame += count;
    if( _isPlaying ) _frame += count;

    _update();
}
