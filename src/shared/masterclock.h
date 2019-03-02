#ifndef MASTERCLOCK_H
#define MASTERCLOCK_H
#include "shared/constants.h"
#include "shared/typedefs.h"


class MasterClock
{
public:
    MasterClock() { }
    nFrame engine_frame() { return _engine_frame; }
    nPpqn ppqn() { return _ppqn; }
    nSequenceStep sequence_step() { return _ppqn / 6; }
    double bar();
    double seconds() { return (double)_frame / SAMPLE_RATE; }
    bool is_playing() { return _is_playing; }
    void start() { _frame = 0; _ppqn = 0; _is_playing = true; }
    void stop() { _is_playing = false; }
    void resume() { _is_playing = true; }
    void increment_frame(nFrame count);
    void increment_ppqn(nPpqn count) { _ppqn += count; _update(); }
private:
    void _update() {}
    bool _is_playing = false;
    nFrame _engine_frame = 0;
    nFrame _frame = 0;
    nPpqn _ppqn = 0;
};

#endif // MASTERCLOCK_H
