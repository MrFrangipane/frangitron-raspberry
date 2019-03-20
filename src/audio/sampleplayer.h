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
    SamplePlayer(const nFrame bufferSize = 0, SampleBank * const sampleBank = nullptr) :
        AbstractModule(bufferSize),
        _sampleBank(sampleBank)
    { }
    ModuleStatus const status() override;
    void update(ModuleStatus status_) override;
    void process(Sample const * bufferIn, const nFrame time) override;
    void gate(nFrame time) override;
private:
    float _amplitude = 0;
    int _sampleIndex = 0;
    _Envelope _envAmplitude;
    _Envelope _envSidechain;
    SampleBank * const _sampleBank;
};


#endif // SAMPLEPLAYER_H
