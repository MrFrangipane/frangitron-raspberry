#include "_aweighting.h"


_AWeighting::_AWeighting()
{
    _filter_hi.setMode(HIPASS);
    _filter_hi.setCutoff(0.69);
    _filter_hi.setResonance(0.3);

    _filter_low_1.setCutoff(0.69);
    _filter_low_1.setResonance(0.0);

    _filter_low_2.setCutoff(0.69);
    _filter_low_2.setResonance(0.0);
}


Sample _AWeighting::process(Sample sample)
{
    _sample = _filter_hi.process(sample);
    _sample = _filter_low_1.process(_sample);
    _sample = _filter_low_2.process(_sample);

    return _sample * A_WEIGHT_GAIN;
}
