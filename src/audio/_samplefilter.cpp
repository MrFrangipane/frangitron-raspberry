#include "_samplefilter.h"

Sample _SampleFilter::process(Sample input)
{
    // TWO POLE
    _buf0 += _realCutoff * (input - _buf0 + _feedbackAmount * (_buf0 - _buf2));
    _buf1 += _realCutoff * (_buf0 - _buf1);

    // FOUR POLE
    _buf2 += _realCutoff * (_buf1 - _buf2);
    _buf3 += _realCutoff * (_buf2 - _buf3);

    switch (_mode)
    {
        case LOWPASS:
            return _buf3;

        case HIPASS:
            return (input - _buf3);

        case BANDPASS:
            return (_buf0 - _buf2);

        default:
            return 0.0;
    }
}

void _SampleFilter::setCutoff(float cutoff)
{
     _cutoff = cutoff;
     _realCutoff = exp(-1.0 / _cutoff) * exp(1);
     _realCutoff = (REAL_CUTOFF_MAX - REAL_CUTOFF_MIN) * _realCutoff + REAL_CUTOFF_MIN;
     _calculateFeedbackAmount();
}
