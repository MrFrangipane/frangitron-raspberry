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
    Filter filter;
    float volume = 1.0;
    void process(const Sample * input, Sample * output, nFrame time);
private:
    nFrame _bufferSize = 0;
};

#endif // TRACK_H
