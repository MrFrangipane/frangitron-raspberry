#include "filter.h"

void Filter::process(Sample const * bufferIn, Sample * bufferOut, nFrame time)
{
    _time = time;
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = bufferIn[_left];
        bufferOut[_right] = bufferIn[_right];

        _time++;
    }
}
