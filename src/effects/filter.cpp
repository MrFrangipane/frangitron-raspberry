#include "filter.h"

double _SampleFilter::process(Sample input)
{
    _buf0 += _realCutoff * (input - _buf0 + _feedbackAmount * (_buf0 - _buf2));
    _buf1 += _realCutoff * (_buf0 - _buf1);
    _buf2 += _realCutoff * (_buf1 - _buf2);
    _buf3 += _realCutoff * (_buf2 - _buf3);

    switch (_mode)
    {
        case LOWPASS:
            return _buf3 / 2.0;

        case HIGHPASS:
            return (input - _buf3) / 2.0;

        case BANDPASS:
            return (_buf0 - _buf2) / 2.0;

        default:
            return input / 2.0;
    }
}


void _SampleFilter::update(FilterStatus status_)
{
    _mode = status_.mode;

    _cutoff = status_.cutoff;
    _realCutoff = exp(-1.0 / _cutoff) * exp(1);
    _realCutoff = (REAL_CUTOFF_MAX - REAL_CUTOFF_MIN) * _realCutoff + REAL_CUTOFF_MIN;

    _resonance = status_.resonance;

    _calculateFeedbackAmount();
}


FilterStatus _SampleFilter::status()
{
    FilterStatus status_;

    status_.mode = _mode;
    status_.cutoff = _cutoff;
    status_.resonance = _resonance;

    return status_;
}


void Filter::process(Sample const * bufferIn, Sample * bufferOut, const nFrame time)
{
    for( nFrame i = 0; i < _bufferSize; i++ ) {
        _left = i * 2;
        _right = _left + 1;

        bufferOut[_left] = _filterL.process(bufferIn[_left]);
        bufferOut[_right] = _filterR.process(bufferIn[_right]);
    }
}
