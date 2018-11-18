#ifndef FILTER_H
#define FILTER_H


#include "typedefs.h"


class Filter
{
public:
    Filter() {}
    Filter(const nFrame bufferSize) : _bufferSize(bufferSize) {}
    void process(Sample const * bufferIn, Sample * bufferOut, const nFrame time);
    float freq = 0.0;
private:
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
};

#endif // FILTER_H
