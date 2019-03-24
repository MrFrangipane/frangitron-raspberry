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
    KickSynth(const nFrame bufferSize = 0, const bool isRoutedToMasterBus = false);
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const ClockStatus time) override;
    void gate(ClockStatus time) override;
private:
    float _pitch = 65.0;
    float _hardSoft = 0.05;
    float _release = 1.0;
    int _sampleIndex = 0;
    float _sampleAmount = 1.0;
    float _sidechainAmount = 0.7;
    float _amplitude = 0.0;
    float _pichitude = 0.0;
    _Envelope _envPitch;
    _Envelope _envAmplitude;
    _SampleFilter _hiPass1;
    _SampleFilter _hiPass2;
};


#endif // KICKSYNTH_H
