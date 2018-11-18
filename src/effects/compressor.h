#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "levelmeter.h"

struct CompressorStatus {
    float level = 0.0;
    bool gate = false;
    float threshold = 0.0;
    float attack = 0.0;
    float release = 0.0;
    float ratio = 0.0;
    float gain = 0.0;
};

class Compressor
{
public:
    Compressor() {}
    Compressor(const nFrame bufferSize) : _bufferSize(bufferSize) {}
    CompressorStatus status();
    void setStatus(CompressorStatus status_);
    void process(Sample const * bufferIn, Sample * bufferOut, const nFrame time);
    float level() { return _level; }
    bool gate() { return _gate; }
    float threshold = -35.0;
    float attack = 0.1;
    float release = 0.5;
    float ratio = 4.0;
    float gain = 2.0;
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
    bool _gate = false;
    LevelMeter _levelMeter;
};

#endif // COMPRESSOR_H
