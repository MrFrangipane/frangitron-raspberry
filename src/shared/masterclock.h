#ifndef MASTERCLOCK_H
#define MASTERCLOCK_H

#include <cmath>
#include "shared/constants.h"
#include "shared/typedefs.h"


struct ClockStatus {
    bool isPlaying = false;
    double tempo = 0.0;
    nSecond seconds = 0.0;
    nBar bar = 0;
    nBeat beat = 0;
    nStep step = 0;
    nPulse pulse = 0;
    nPulse pulseLatencyCompensated = 0;
    nFrame frame = 0;
    nFrame engineFrame = 0;
    nPulse enginePulse = 0;
    nFrame barAsFrame = 0;
    nFrame framePerBar = 0;
};


class MasterClock
{
public:
    MasterClock() { }
    bool isPlaying() { return _status.isPlaying; }
    ClockStatus status() { return _status; }
    void start() { _status.frame = 0; _status.isPlaying = true; }
    void stop() { _status.isPlaying = false; }
    void resume() { _status.isPlaying = true; }
    void setTempo(float tempo);
    void incrementFrame(nFrame count);

private:
    void _update();
    ClockStatus _status;
    double _pulsePerSecond = 0.0;
};

#endif // MASTERCLOCK_H
