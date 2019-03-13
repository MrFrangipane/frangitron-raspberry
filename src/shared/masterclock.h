#ifndef MASTERCLOCK_H
#define MASTERCLOCK_H
#include "shared/constants.h"
#include "shared/typedefs.h"


class MasterClock
{
public:
    MasterClock() { }
    nFrame engineFrame() { return _engineFrame; }
    nPpqn ppqnCount() { return _ppqnCount; }
    nSequenceStep sequenceStep() { return _ppqnCount / 6; }
    double bar();
    double seconds() { return (double)_frame / SAMPLE_RATE; }
    bool isPlaying() { return _isPlaying; }
    void start() { _frame = 0; _ppqnCount = 0; _isPlaying = true; }
    void stop() { _isPlaying = false; }
    void resume() { _isPlaying = true; }
    void incrementFrame(nFrame count);
    void incrementPpqn(nPpqn count) { _ppqnCount += count; _update(); }
private:
    void _update() {}
    bool _isPlaying = false;
    nFrame _engineFrame = 0;
    nFrame _frame = 0;
    nPpqn _ppqnCount = 0;
};

#endif // MASTERCLOCK_H
