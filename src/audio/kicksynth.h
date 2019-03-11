#ifndef KICKSYNTH_H
#define KICKSYNTH_H

#define ATTACK_FACTOR 500.0

#include <cmath>
#include "shared/typedefs.h"
#include "audio/_envelope.h"
#include "audio/_samplemeter.h"
#include "audio/_samplefilter.h"
#include "audio/abstractmodule.h"


class KickSynth : public AbstractModule
{
public:
    KickSynth(const nFrame bufferSize = 0);
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time, const SampleBank * /*sampleBank*/) override;
    void gate(nFrame time) override;
private:
    float _pitch = 65.0;
    float _hard_soft = 0.05;
    float _release = 1.0;
    int _sample_index = 0;
    float _sample_amount = 1.0;
    float _sidechain_amount = 0.7;
    float _amplitude = 0.0;
    float _pichitude = 0.0;
    _Envelope _envelope_pitch;
    _Envelope _envelope_amplitude;
    _SampleFilter _hipass_1;
    _SampleFilter _hipass_2;
};


#endif // KICKSYNTH_H
