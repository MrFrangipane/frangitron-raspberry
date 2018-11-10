#ifndef FILTER_H
#define FILTER_H


#include "typedefs.h"


class Filter
{
public:
    Filter() {}
    Filter(const nFrame bufferSize) : _bufferSize(bufferSize) {}
    void process(Sample const * input, Sample * output, nFrame time);
private:
    nFrame _bufferSize = 0;
    nFrame _time = 0;
};

#endif // FILTER_H
