#include "filter.h"

void Filter::process(Sample const * input, Sample * output, nFrame time)
{
    _time = time;
    for( int i = 0; i < _bufferSize; i++ ) {

        output[i] = input[i] * 1.5;

        _time++;
    }
}
