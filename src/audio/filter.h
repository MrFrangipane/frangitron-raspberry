#ifndef FILTER_H
#define FILTER_H

#include "shared/typedefs.h"
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

    Filter(const nFrame bufferSize = 0, const bool isRoutedToMaster = false) :
        AbstractModule(bufferSize, isRoutedToMaster),
        _lowPassL(_SampleFilterMode::LOWPASS),
        _lowPassR(_SampleFilterMode::LOWPASS),
        _hiPassL(_SampleFilterMode::HIPASS),
        _hiPassR(_SampleFilterMode::HIPASS) { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const ClockStatus /*time*/) override;
private:
    FilterMode _mode = FilterMode::PASSTHROUGH;
    _SampleFilter _lowPassL;
    _SampleFilter _lowPassR;
    _SampleFilter _hiPassL;
    _SampleFilter _hiPassR;
};

#endif // FILTER_H
