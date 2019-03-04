#ifndef KICKSYNTH_H
#define KICKSYNTH_H

#include <cmath>
#include "shared/typedefs.h"
#include "audio/_adr.h"
#include "audio/_samplemeter.h"
#include "audio/_samplefilter.h"
#include "audio/abstractmodule.h"


class KickSynth : public AbstractModule
{
public:
    KickSynth(const nFrame bufferSize = 0);
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time) override;
    void gate(nFrame time) override;
private:
    float _pitch = 160.0;
    float _sidechain_amount = 0.7;
    float _amplitude = 0.0;
    _ADR _envelope_pitch;
    _ADR _envelope_amplitude;
    _SampleFilter _hipass;
};


#endif // KICKSYNTH_H
