#include "filter.h"

void Filter::process(Sample const * input, Sample * output, nFrame time)
{
    _time = time;
    for( int i = 0; i < _bufferSize; i++ ) {

        output[i] = input[i];

        _time++;
    }
}
