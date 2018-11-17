#include "track.h"


Track::Track(nFrame bufferSize) : _bufferSize(bufferSize)
{
    // Swap buffers
    _bufferFilter = new Sample[bufferSize * 2];
    _bufferComp = new Sample[bufferSize * 2];

    // Effects
    filter = Filter(bufferSize);
    compressor = Compressor(bufferSize);
}

Track::~Track() {
    if( _bufferFilter ) delete[] _bufferFilter;
    if( _bufferComp ) delete[] _bufferComp;
}

void Track::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    levelMeterIn.bufferBegin();
    levelMeterOut.bufferBegin();
    filter.process(bufferIn, _bufferFilter, time);
    compressor.process(_bufferFilter, _bufferComp, time);

    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = bufferIn[_left];
        bufferOut[_right] = bufferIn[_right];

        levelMeterIn.bufferStep(bufferIn[_left], bufferIn[_right]);
        levelMeterOut.bufferStep(bufferOut[_left], bufferOut[_right]);

        _time++;
    }

    levelMeterIn.bufferEnd();
    levelMeterOut.bufferEnd();
}
