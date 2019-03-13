#include "_aweighting.h"


_AWeighting::_AWeighting()
{
    _filterHi.setMode(HIPASS);
    _filterHi.setCutoff(0.69);
    _filterHi.setResonance(0.3);

    _filterLow1.setCutoff(0.69);
    _filterLow1.setResonance(0.0);

    _filterLow2.setCutoff(0.69);
    _filterLow2.setResonance(0.0);
}


Sample _AWeighting::process(Sample sample)
{
    _sample = _filterHi.process(sample);
    _sample = _filterLow1.process(_sample);
    _sample = _filterLow2.process(_sample);

    return _sample * A_WEIGHT_GAIN;
}
