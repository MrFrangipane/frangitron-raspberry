#include "masterclock.h"

double MasterClock::bar()
{
    int note = _ppqn / 24;
    int bar = note / 4;
    return bar + (double)(note % 4) * 0.1;
}


void MasterClock::increment_frame(nFrame count)
{
    _engine_frame += count;
    if( is_playing() ) _frame += count;
    _update();
}
