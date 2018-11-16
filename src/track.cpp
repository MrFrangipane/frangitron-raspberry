#include "track.h"


Track::Track(nFrame bufferSize) : _bufferSize(bufferSize)
{
    // Swap buffers
    _bufferA = new Sample[_bufferSize];
    _bufferB = new Sample[_bufferSize];

    // Effects
    filter = Filter(bufferSize);
    compressor = Compressor(bufferSize);
}

Track::~Track() {
    if( _bufferA ) delete[] _bufferA;
    if( _bufferB ) delete[] _bufferB;
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, nFrame time)
{
    levelMeterIn.bufferBegin();
    levelMeterOut.bufferBegin();

    filter.process(bufferIn, _bufferA, time);
    compressor.process(_bufferA, _bufferB, time);

    _time = time;
    for( int i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _bufferB[_left] * volume;
        bufferOut[_right] = _bufferB[_right] * volume;

        levelMeterIn.bufferStep(bufferIn[_left], bufferIn[_right]);
        levelMeterOut.bufferStep(bufferOut[_left], bufferOut[_right]);

        _time++;
    }

    levelMeterIn.bufferEnd();
    levelMeterOut.bufferEnd();
}
