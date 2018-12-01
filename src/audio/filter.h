#ifndef FILTER_H
#define FILTER_H

#include "typedefs.h"
#include "audio/abstractmodule.h"
#include "audio/_samplefilter.h"

class Filter : public AbstractModule
{
public:
    enum FilterMode {
        LOWPASS = 0,
        HIPASS,
        PASSTHROUGH,
        nFilterMode
    };

    Filter(const nFrame bufferSize = 0) :
        AbstractModule(bufferSize),
        _lowPassL(_SampleFilterMode::LOWPASS),
        _lowPassR(_SampleFilterMode::LOWPASS),
        _hiPassL(_SampleFilterMode::HIPASS),
        _hiPassR(_SampleFilterMode::HIPASS) { }
    Status const status() override;
    void update(Status status_) override;
    void process(Sample const * bufferIn, const nFrame /*time*/) override;
private:
    FilterMode _mode = FilterMode::PASSTHROUGH;
    _SampleFilter _lowPassL;
    _SampleFilter _lowPassR;
    _SampleFilter _hiPassL;
    _SampleFilter _hiPassR;
};

#endif // FILTER_H
