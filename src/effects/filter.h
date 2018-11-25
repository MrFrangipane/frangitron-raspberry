#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include "typedefs.h"


#define REAL_CUTOFF_MIN 0.005
#define REAL_CUTOFF_MAX 0.98


enum FilterMode {
    LOWPASS = 0,
    HIPASS,
    BANDPASS,
    nFilterMode
};


struct FilterStatus {
    FilterMode mode = FilterMode::HIPASS;
    float cutoff = 0.0;
    float resonance = 0.1;
};


class _SampleFilter
{
public:
    _SampleFilter(FilterMode mode = LOWPASS) :
        _cutoff(1.0),
        _resonance(0.10),
        _realCutoff(REAL_CUTOFF_MAX),
        _mode(mode),
        _buf0(0.0),
        _buf1(0.0),
        _buf2(0.0),
        _buf3(0.0)
    {
        _initCutoff();
        _calculateFeedbackAmount();
    }
    double process(Sample input);
    void update(FilterStatus status_);
    FilterStatus status();

private:
    float _cutoff;
    float _resonance;
    float _realCutoff;
    FilterMode _mode;
    float _feedbackAmount;
    void _calculateFeedbackAmount() { _feedbackAmount = fmin(_resonance + _resonance / (1.0 - _realCutoff), 1.0); }
    void _initCutoff() { if (_mode == LOWPASS) { _cutoff = 1.0; } else if (_mode == HIPASS) { _cutoff = 0.0; } else { _cutoff = 0.5; } }
    Sample _buf0;
    Sample _buf1;
    Sample _buf2;
    Sample _buf3;
};


class Filter
{
public:
    Filter() {}
    Filter(const nFrame bufferSize, FilterMode mode) :
        _bufferSize(bufferSize),
        _filterL(mode),
        _filterR(mode)
    {
        _bufferOut.reserve(bufferSize * 2);
    }
    FilterStatus status() { return _filterL.status() ;}
    void update(FilterStatus status_) { _filterL.update(status_); _filterR.update(status_); }
    Sample const * bufferOut() { return _bufferOut.data(); }
    void process(Sample const * bufferIn, const nFrame time);
private:
    Buffer _bufferOut;
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    _SampleFilter _filterL;
    _SampleFilter _filterR;
};

#endif // FILTER_H
