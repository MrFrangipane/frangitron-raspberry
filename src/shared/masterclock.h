#ifndef MASTERCLOCK_H
#define MASTERCLOCK_H
#include "shared/constants.h"
#include "shared/typedefs.h"


struct ClockStatus {
    bool isPlaying = false;
    double seconds = 0.0;
    double bar = 0;
    nSequenceStep sequenceStep = 0;
    nFrame frame = 0;
    nFrame engineFrame = 0;
};


class MasterClock
{
public:
    MasterClock() { }
    bool isPlaying() { return _isPlaying; }
    ClockStatus status() { return _status; }
    nPpqn ppqnCount() { return _ppqnCount; }
    void start() { _frame = 0; _ppqnCount = 0; _isPlaying = true; }
    void stop() { _isPlaying = false; }
    void resume() { _isPlaying = true; }
    void incrementFrame(nFrame count);
    void incrementPpqn(nPpqn count) { _ppqnCount += count; _update(); }
private:
    void _update();
    ClockStatus _status;
    bool _isPlaying = false;
    nFrame _engineFrame = 0;
    nFrame _frame = 0;
    nPpqn _ppqnCount = 0;
};

#endif // MASTERCLOCK_H
