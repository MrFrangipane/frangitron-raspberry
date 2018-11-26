#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include "typedefs.h"


#define REAL_CUTOFF_MIN 0.005
#define REAL_CUTOFF_MAX 0.98


enum _SampleFilterMode {
    LOWPASS = 0,
    HIPASS,
    BANDPASS,
    n_SampleFilterMode
};


class _SampleFilter
{
public:
    _SampleFilter(_SampleFilterMode mode = LOWPASS) :
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
    float cutoff() { return _cutoff; }
    void setCutoff(float cutoff) { _cutoff = cutoff; _calculateFeedbackAmount(); }
    float resonance() { return _resonance; }
    void setResonance(float resonance) { _resonance = resonance; _calculateFeedbackAmount(); }

private:
    float _cutoff;
    float _resonance;
    float _realCutoff;
    _SampleFilterMode _mode;
    float _feedbackAmount;
    void _calculateFeedbackAmount() { _feedbackAmount = fmin(_resonance + _resonance / (1.0 - _realCutoff), 1.0); }
    void _initCutoff() { if (_mode == LOWPASS) { _cutoff = 1.0; } else if (_mode == HIPASS) { _cutoff = 0.0; } else { _cutoff = 0.5; } }
    Sample _buf0;
    Sample _buf1;
    Sample _buf2;
    Sample _buf3;
};


struct FilterStatus {
    float cutoff = 0.0;
    float resonance = 0.1;
};


class Filter
{
public:
    enum FilterMode {
        LOWPASS = 0,
        HIPASS,
        PASSTHROUGH,
        nFilterMode
    };

    Filter() {}
    Filter(const nFrame bufferSize) :
        _bufferSize(bufferSize),
        _lowPassL(_SampleFilterMode::LOWPASS),
        _lowPassR(_SampleFilterMode::LOWPASS),
        _hiPassL(_SampleFilterMode::HIPASS),
        _hiPassR(_SampleFilterMode::HIPASS)
    {
        _bufferOut.reserve(bufferSize * 2);
    }
    FilterStatus status();
    void update(FilterStatus status_);
    Sample const * bufferOut() { return _bufferOut.data(); }
    void process(Sample const * bufferIn, const nFrame time);
private:
    Buffer _bufferOut;
    nFrame _bufferSize = 0;
    nFrame _left = 0;
    nFrame _right = 0;
    nFrame _time = 0;
    FilterMode _mode = FilterMode::PASSTHROUGH;
    _SampleFilter _lowPassL;
    _SampleFilter _lowPassR;
    _SampleFilter _hiPassL;
    _SampleFilter _hiPassR;
};

#endif // FILTER_H
