#include "track.h"


Track::Track(nFrame bufferSize) : _bufferSize(bufferSize)
{
    // Swap buffers
    _bufferA = new Sample[_bufferSize];
    _bufferB = new Sample[_bufferSize];

    // Effects
    filter = Filter(bufferSize);

}

Track::~Track() {
    if( _bufferA ) delete[] _bufferA;
    if( _bufferB ) delete[] _bufferB;
}

void Track::process(Sample const * input, Sample * output, nFrame time)
{
    filter.process(input, _bufferA, time);
    filter.process(_bufferA, _bufferB, time);
    filter.process(_bufferB, _bufferA, time);
    filter.process(_bufferA, _bufferB, time);

    for( int i = 0; i < _bufferSize; i++ ) {
        output[i] = _bufferB[i];
    }
}
