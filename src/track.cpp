#include "track.h"


Track::Track(nFrame bufferSize) : _bufferSize(bufferSize)
{
    filter = Filter(bufferSize);
}


void Track::process(Sample const * input, Sample * output, nFrame time)
{
    Sample *bufferA = new Sample[_bufferSize];
    Sample *bufferB = new Sample[_bufferSize];

    filter.process(input, bufferA, time);
    filter.process(bufferA, bufferB, time);
    filter.process(bufferB, bufferA, time);
    filter.process(bufferA, bufferB, time);

    for( int i = 0; i < _bufferSize; i++ ) {
        output[i] = input[i];
    }
}
