#include "masterclock.h"

double MasterClock::bar()
{
    int note = _ppqnCount / 24;
    int bar = note / 4;
    return bar + (double)(note % 4) * 0.1;
}


void MasterClock::incrementFrame(nFrame count)
{
    _engineFrame += count;
    if( isPlaying() ) _frame += count;
    _update();
}
