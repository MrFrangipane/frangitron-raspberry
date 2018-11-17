#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cmath>
#include "levelmeter.h"

class Compressor
{
public:
    Compressor() {}
    Compressor(const nFrame bufferSize) : _bufferSize(bufferSize) {}
    void process(Sample const * bufferIn, Sample * bufferOut, nFrame time);
    float level = 1.0;
private:
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    float _rmsMono = 0.0;
    float _threshold = -30.0;
    float _attack = 0.01;
    float _release = 0.002;
    float _ratio = 4.0;
    LevelMeter _levelMeter;
};

#endif // COMPRESSOR_H
