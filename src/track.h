#ifndef TRACK_H
#define TRACK_H

#include <cstdint>
#include "typedefs.h"
#include "levelmeter.h"
#include "effects/filter.h"
#include "effects/compressor.h"


class Track
{
public:
    Track();
    Track(nFrame bufferSize);
    ~Track();
    Filter filter;
    Compressor compressor;
    float volume = 1.0;
    LevelMeter levelMeterIn;
    LevelMeter levelMeterOut;
    void process(const Sample * bufferIn, Sample * bufferOut, const nFrame time);
private:
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    Sample* _bufferFilter = NULL;
    Sample* _bufferComp = NULL;
};

#endif // TRACK_H
