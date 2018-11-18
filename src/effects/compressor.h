#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "levelmeter.h"

class Compressor
{
public:
    Compressor() {}
    Compressor(const nFrame bufferSize) : _bufferSize(bufferSize) {}
    void process(Sample const * bufferIn, Sample * bufferOut, const nFrame time);
    float level() { return _level; }
    float threshold = -30.0;
    float attack = 0.04;
    float release = 0.002;
private:
    nFrame _bufferSize = 0;
    nFrame _nCycles = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    nFrame _nLerp = 0;
    float _level = 1.0;
    float _levelPrevious = 0.0;
    float _levelTarget = 0.0;
    float _rmsMono = 0.0;
    float _ratio = 4.0;
    LevelMeter _levelMeter;
};

#endif // COMPRESSOR_H
