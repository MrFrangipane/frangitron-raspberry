#ifndef SAMPLEPLAYER_H
#define SAMPLEPLAYER_H

#include <cmath>
#include "shared/typedefs.h"
#include "shared/constants.h"
#include "audio/_envelope.h"
#include "audio/_samplemeter.h"
#include "audio/abstractmodule.h"
#include "audio/_samplebank.h"


class SamplePlayer : public AbstractModule
{
public:
    SamplePlayer(
        const nFrame bufferSize = 0,
        SampleBank * const sampleBank = nullptr,
        const bool isRoutedToMaster = false
    ) :
        AbstractModule(bufferSize, isRoutedToMaster),
        _sampleBank(sampleBank)
    { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const ClockStatus time) override;
    void gate(ClockStatus time) override;
private:
    float _amplitude = 0;
    int _sampleIndex = 0;
    _Envelope _envAmplitude;
    _Envelope _envSidechain;
    SampleBank * const _sampleBank;
};


#endif // SAMPLEPLAYER_H
