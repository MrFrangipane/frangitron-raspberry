#ifndef TRACK_H
#define TRACK_H

#include <cstdint>
#include "typedefs.h"
#include "effects/filter.h"


class Track
{
public:
    Track() {}
    Track(nFrame bufferSize);
    ~Track();
    Filter filter;
    float volume = 1.0;
    void process(const Sample * input, Sample * output, nFrame time);
private:
    nFrame _bufferSize = 0;
    Sample* _bufferA;
    Sample* _bufferB;
};

#endif // TRACK_H
