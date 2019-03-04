#ifndef _SAMPLEFILTER_H
#define _SAMPLEFILTER_H

#include <cmath>
#include "shared/typedefs.h"

#define REAL_CUTOFF_MIN 0.005
#define REAL_CUTOFF_MAX 0.98
#define FEEDBACK_MAX 1.2  // Resonance max = 0.5 * FEEDBACK_MAX

enum _SampleFilterMode {
    LOWPASS = 0,
    HIPASS,
    BANDPASS,
    n_SampleFilterMode
};


class _SampleFilter
{
public:
    _SampleFilter(_SampleFilterMode mode=_SampleFilterMode::LOWPASS) :
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
    Sample process(Sample input);
    float cutoff() { return _cutoff; }
    void setCutoff(float cutoff);
    float resonance() { return _resonance; }
    void setResonance(float resonance) { _resonance = resonance; _calculateFeedbackAmount(); }
    float feedback() { return _feedbackAmount; }
    float overhead() { return 1.0 / fmax(1.0, _feedbackAmount); }
    void setMode(_SampleFilterMode mode) { _mode = mode; }

private:
    float _cutoff;
    float _resonance;
    float _realCutoff;
    _SampleFilterMode _mode;
    float _feedbackAmount;
    void _calculateFeedbackAmount() { _feedbackAmount = fmin(_resonance + _resonance / (1.0 - _realCutoff), FEEDBACK_MAX); }
    void _initCutoff() { if (_mode == LOWPASS) { _cutoff = 1.0; } else if (_mode == HIPASS) { _cutoff = 0.0; } else { _cutoff = 0.5; } }
    Sample _buf0;
    Sample _buf1;
    Sample _buf2;
    Sample _buf3;
};

#endif // _SAMPLEFILTER_H
