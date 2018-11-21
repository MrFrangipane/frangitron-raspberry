#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include "typedefs.h"


#define REAL_CUTOFF_MIN 0.005
#define REAL_CUTOFF_MAX 0.98


enum FilterMode {
    LOWPASS = 0,
    HIGHPASS,
    BANDPASS,
    nFilterMode
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
    void setCutoff(float cutoff);
    void setResonance(float resonance) {_resonance = resonance; _calculateFeedbackAmount(); }
    void setMode(FilterMode mode) {_mode = mode; }

private:
    float _cutoff;
    float _resonance;
    float _realCutoff;
    FilterMode _mode;
    float _feedbackAmount;
    void _calculateFeedbackAmount() { _feedbackAmount = fmin(_resonance + _resonance / (1.0 - _realCutoff), 1.0); }
    void _initCutoff() { if (_mode == LOWPASS) { _cutoff = 1.0; } else if (_mode == HIGHPASS) { _cutoff = 0.0; } else { _cutoff = 0.5; } }
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
    {}
    void process(Sample const * bufferIn, Sample * bufferOut, const nFrame time);
    void setCutoff(float cutoff) { _filterL.setCutoff(cutoff); _filterR.setCutoff(cutoff); }
    void setResonance(float resonance) { _filterL.setResonance(resonance); _filterR.setResonance(resonance); }
    void setMode(FilterMode mode) { _filterL.setMode(mode); _filterR.setMode(mode); }
private:
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    _SampleFilter _filterL;
    _SampleFilter _filterR;
};

#endif // FILTER_H
