#ifndef MASTERCLOCK_H
#define MASTERCLOCK_H

#include <cmath>
#include "shared/constants.h"
#include "shared/typedefs.h"


struct ClockStatus {
    bool isPlaying = false;
    bool isAtBar = true;
    nSecond seconds = 0.0;
    double bar = 0;
    float tempo = 0.0;
    nSequenceStep sequenceStep = 0.0;
    nFrame frame = 0;
    nFrame engineFrame = 0;
    nFrame lastBarFrame = 0;
    nFrame framePerBar = 0;
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
    void incrementPpqn();
private:
    void _update();
    ClockStatus _status;
    bool _isPlaying = false;
    nFrame _engineFrame = 0;
    nFrame _frame = 0;
    nPpqn _ppqnCount = 0;
    nFrame _lastPpqnUpdate = 0;
    float _ppqnPerFrame = 0.0;
    double _tempoAveraging = 0.0;
    nSecond _lastTempoAveraging = 0;
    int _tempoCycles = 0;
};

#endif // MASTERCLOCK_H
